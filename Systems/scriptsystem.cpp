#include "scriptsystem.h"

#include <QJsonArray>
#include "Systems/engine.h"
#include "Systems/objectmanager.h"
#include "Systems/filemanager.h"
#include "Systems/inputsystem.h"
#include "Systems/transformsystem.h"
#include "Systems/collisionsystem.h"
#include "Components/scriptcomponent.h"
#include "Components/meshcomponent.h"
#include "Components/audiocomponent.h"
#include "constants.h"

ScriptSystem::ScriptSystem()
{

}

void ScriptSystem::beginPlay()
{
    auto scriptComps = Engine::getInstance()->mObjectManager->scriptComps;
    for(auto& sC : scriptComps)
    {
        callJSFunction(sC, "beginPlay");
    }
}

void ScriptSystem::tick()
{
    auto scriptComps = Engine::getInstance()->mObjectManager->scriptComps;
    for(auto& sC : scriptComps)
    {
        callJSFunction(sC, "tick");
    }
}

void ScriptSystem::endPlay()
{
    auto scriptComps = Engine::getInstance()->mObjectManager->scriptComps;
    for(auto& sC : scriptComps)
    {
        callJSFunction(sC, "endPlay");
    }
}

void ScriptSystem::readScript(ScriptComponent* sC)
{
    sC->mEngine = new QJSEngine();
    //sC->mEngine->setProperty("engine", sC->mEngine->newQObject(this));

    //Make a name for the object in the script engine
    sC->mEngine->globalObject().setProperty("engine", sC->mEngine->newQObject(this));
    sC->mEngine->globalObject().setProperty("ID", sC->eID);

    QFile file(QString::fromStdString(gsl::scriptFilePath) + sC->mFileName);
    file.open(QFile::ReadOnly);

    QTextStream stream(&file);
    QString string = stream.readAll();

    QJSValue value = sC->mEngine->evaluate(string, QString::fromStdString(gsl::scriptFilePath) + sC->mFileName);
}

void ScriptSystem::callJSFunction(ScriptComponent* sC, QString functionName)
{
    if(sC && sC->mFileName != "")
    {
        if(!sC->mEngine)
        {
            readScript(sC);
        }
        auto& fullFileName =  QString::fromStdString(gsl::scriptFilePath)+ sC->mFileName;
        sC->mEngine->evaluate(Engine::getInstance()->mFileManager->readScriptFile(fullFileName), fullFileName);
        QJSValue func = sC->mEngine->evaluate(functionName);
        QJSValue result = func.call();
        //qDebug() << result.toString();
    }
}
void ScriptSystem::callJSFunction(ScriptComponent* sC, QString functionName, QJSValueList args)
{

    if(sC && sC->mFileName != "")
    {
        if(!sC->mEngine)
        {
            readScript(sC);
        }
            //qDebug() << "Calling collision on entity " << sC->eID;
        auto& fullFileName =  QString::fromStdString(gsl::scriptFilePath)+ sC->mFileName;
        sC->mEngine->evaluate(Engine::getInstance()->mFileManager->readScriptFile(fullFileName), fullFileName);
        QJSValue func = sC->mEngine->evaluate(functionName);
        QJSValue result = func.call(args);
        //qDebug() << result.toString();
    }
}

void ScriptSystem::collision(ObjectHit hit)
{
    ScriptComponent* s = Engine::getInstance()->mObjectManager->scriptComps[hit.id1];
    QJSValueList args;
    if(s)
    {
        args << hit.id2;
        callJSFunction(s, "onEntityCollision", args);
    }

    s = Engine::getInstance()->mObjectManager->scriptComps[hit.id2];
    if(s)
    {
        args.clear();
        args << hit.id1;
        callJSFunction(s,"onEntityCollision", args);
    }
}

//invokables:

int ScriptSystem::getPlayerID()
{
    return Engine::getInstance()->mInputSystem->possessedEntity;
}

void ScriptSystem::setPlayerID(int inID)
{
    Engine::getInstance()->mInputSystem->possessedEntity = inID;
}

void ScriptSystem::playSound(int inID)
{
    AudioComponent* a = Engine::getInstance()->mObjectManager->audioComps[inID];
    if(a && a->mSoundSource)
        a->mSoundSource->play();
}

void ScriptSystem::addInput(unsigned inID, QJsonArray moveArr)
{
    gsl::Vector3D moveDir = gsl::Vector3D(moveArr[0].toDouble(), moveArr[1].toDouble(), moveArr[2].toDouble());
    Engine::getInstance()->mInputSystem->sendMoveInput(inID,moveDir);
}

QJsonArray ScriptSystem::getPosition(unsigned inID)
{
    gsl::Vector3D pos = Engine::getInstance()->mTransformSystem->getWorldPosition(inID);
    QJsonArray posArr;
    posArr.push_back(pos.x);
    posArr.push_back(pos.y);
    posArr.push_back(pos.z);
    return posArr;
}

int ScriptSystem::randInRange(int min, int max)
{
    int r = (std::rand()%(max-min)) + min;
    return r;
}

void ScriptSystem::setPosition(QJsonArray inPos, unsigned inID)
{
    gsl::Vector3D pos;

    pos.x = inPos[0].toDouble();
    pos.y = inPos[1].toDouble();
    pos.z = inPos[2].toDouble();

    Engine::getInstance()->mTransformSystem->setPosition(inID, pos);

}

void ScriptSystem::setVisibility(bool value, unsigned inID)
{
    MeshComponent* m = Engine::getInstance()->mObjectManager->meshComps[inID];
    if(m)
        m->bIsVisible = value;
}

void ScriptSystem::debug(QString string)
{
    qDebug() << "JS DEBUG: " << string;
}
