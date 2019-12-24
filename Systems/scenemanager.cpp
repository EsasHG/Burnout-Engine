#include "scenemanager.h"

#include "Systems/engine.h"
#include "Systems/objectmanager.h"
#include "Systems/rendersystem.h"
#include "Systems/camerasystem.h"
#include "Systems/landscapemanager.h"
#include "Systems/transformsystem.h"
#include "Sound/soundmanager.h"
#include "Components/collisioncomponent.h"
#include "Components/audiocomponent.h"
#include "Components/audiocomponent.h"
#include "Components/scriptcomponent.h"
#include "Components/cameracomponent.h"
#include "Components/transformcomponent.h"
#include "Components/materialcomponent.h"
#include "Components/meshcomponent.h"
#include "Components/physicscomponent.h"
#include "Components/inputcomponent.h"
#include "Components/landscapecomponent.h"
#include "Components/lightcomponent.h"
#include "Rendering/mainwindow.h"
#include "filemanager.h"

#include <vector>

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
SceneManager::SceneManager(MainWindow* inMainWindow)
{
    mMainWindow = inMainWindow;
}

void SceneManager::saveScene(QString fileName)
{
    QFile saveFile(fileName);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
    }
    else
    {
        //setting scene name

        QStringList names = fileName.split("/");
        qDebug() << names.back();
        sceneName = names.back();

        //saving landscapes
        for (auto& l : Engine::getInstance()->mObjectManager->landscapeComps)
        {
            if(l)
            {

            QStringList name = fileName.split(".");
                l->filename  = name.first() + ".bmp";
            qDebug() << l->filename;
            saveHeightMap(l->filename, l);
            }
        }


        //saving scene
        QJsonObject gameObject;
        gameObject["SceneName"] = sceneName;
        QJsonArray entityArray;

        for (unsigned i=0; i< Engine::getInstance()->mObjectManager->transformComps.size(); i++)
        {
            QJsonObject componentObject;

            std::vector<BaseComponent*> components = Engine::getInstance()->mObjectManager->getAllComponentsWithID(i);
            for(unsigned j=0; j< static_cast<int>(COMPONENTS::LENGTH); j++)
            {
                if(components[j])
                    componentObject.insert(BaseComponent::compTypeToString(static_cast<COMPONENTS>(j)), components[j]->toJson());
            }
            QJsonObject entityObject;
            QString name =  Engine::getInstance()->mObjectManager->mNames[i];

            entityObject.insert("ID", QJsonValue(static_cast<int>(i)));
            entityObject.insert("Name",name);
            entityObject.insert("Components", componentObject);
//          entityObject.insert("ShowInEditor", QJsonValue(entityInfo.shouldShowInEditor));
            entityArray.push_back(entityObject);

//            entityArray.push_back(componentArray);
        }
        gameObject.insert("Entities", entityArray);
//        if(Engine::getInstance()->mObjectManager->landscape)
//            gameObject.insert("Landscape ID", static_cast<int>(Engine::getInstance()->mObjectManager->landscape->landscapeComp->eID));
        QJsonDocument saveDoc(gameObject);
        saveFile.write(saveDoc.toJson());
    }
}

bool SceneManager::loadScene(QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "ERROR Scene.load(): Failed to open file at specified path!";
        return false;
    }

    QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    if(!document.isObject())
    {
        qDebug() << "Load Error: Wrong formatting";
        return false;
    }

    QJsonObject mainObject = document.object();
    if(mainObject.isEmpty())
    {
        qDebug() << "Load Error: File is empty.";
        return false;
    }

    //actually starts loading

    sceneName = mainObject["SceneName"].toString();

    Engine::getInstance()->mObjectManager->clearEntities();
    mMainWindow->onLoadScene();
    Engine::getInstance()->mCameraSystem->cameras.clear();

    //need these to setup parent-child relationships in ui later
    //cant do this while loading them cause the parent might not exist yet
    std::vector<int> parents;
    std::vector<int> children;

    for (auto entity : mainObject["Entities"].toArray())
    {
        QJsonObject e =  entity.toObject();
        unsigned eID = Engine::getInstance()->mObjectManager->spawnObject(
                    OBJECTS::ENTITY,
                    Engine::getInstance()->mRenderSystem->mShaderProgram[0],
                    e["Name"].toString()
                );
        for(int i=0; i< static_cast<int>(COMPONENTS::LENGTH); i++)
        {
            COMPONENTS CompType = static_cast<COMPONENTS>(i);
            QString s = BaseComponent::compTypeToString(CompType);
            if(e["Components"].toObject()[s].isObject())
            {
                //qDebug() << "Found component on load " << s << " for ID: " << e["ID"].toInt();
                BaseComponent* c = Engine::getInstance()->mObjectManager->addComponent(CompType, eID);
                c->fromJson(e["Components"].toObject()[s].toObject());
            }
        }
        TransformComponent* t = Engine::getInstance()->mObjectManager->transformComps[eID];
        if(t && t->mParent >=0)
        {
            parents.push_back(t->mParent);
            children.push_back(t->eID);
        }
    }

    Engine::getInstance()->mCameraSystem->setEditorCamera();

    //setting up landscape
    for(auto& landscape : Engine::getInstance()->mObjectManager->landscapeComps)
    {
        if(!landscape) continue;
        Engine::getInstance()->mLandscapeManager->loadLandscape(landscape);
    }
    if(children.size() == parents.size())
    {
        for (int i=0; i<parents.size(); i++)
        {
            mMainWindow->setChildUI(parents[i], children[i]);
        }
    }
    return true;
}



void SceneManager::saveHeightMap(QString fileName, LandscapeComponent* lComp) const
{
    MeshComponent* m = Engine::getInstance()->mObjectManager->meshComps[lComp->eID];
    std::vector<Vertex> vertices =  m->mesh->mVertices;
    QStringList sceneName = Engine::getInstance()->mSceneManager->sceneName.split(".");
    Engine::getInstance()->mFileManager->writeHeightMap(fileName,lComp, vertices);
}
QJsonObject CameraComponent::toJson() const
{
    QJsonObject obj;
    QJsonArray forward;
    forward.push_back(static_cast<double>(mForward.x));
    forward.push_back(static_cast<double>(mForward.y));
    forward.push_back(static_cast<double>(mForward.z));
    obj["Forward"] = forward;

    QJsonArray right;
    right.push_back(static_cast<double>(mRight.x));
    right.push_back(static_cast<double>(mRight.y));
    right.push_back(static_cast<double>(mRight.z));
    obj["Right"] = right;

    QJsonArray up;
    up.push_back(static_cast<double>(mUp.x));
    up.push_back(static_cast<double>(mUp.y));
    up.push_back(static_cast<double>(mUp.z));
    obj["Up"] = up;

    obj.insert("Pitch", static_cast<double>(mPitch));
    obj.insert("Yaw", static_cast<double>(mYaw));
    obj.insert("Speed", static_cast<double>(mSpeed));

    obj.insert("ThirdPerson", bIsThirdPersonCamera);
    obj.insert("EntityToLookAt", entityToLookAt);

    return obj;
}

void CameraComponent::fromJson(QJsonObject obj)
{
    QJsonArray forward = obj["Forward"].toArray();
    mForward.x = forward[0].toDouble();
    mForward.y = forward[1].toDouble();
    mForward.z = forward[2].toDouble();

    QJsonArray right = obj["Right"].toArray();
    mRight.x = right[0].toDouble();
    mRight.y = right[1].toDouble();
    mRight.z = right[2].toDouble();

    QJsonArray up = obj["Up"].toArray();
    mUp.x = up[0].toDouble();
    mUp.y = up[1].toDouble();
    mUp.z = up[2].toDouble();

    mPitch = obj["Pitch"].toDouble();
    mYaw = obj["Yaw"].toDouble();
    mSpeed = obj["Speed"].toDouble();

    bIsThirdPersonCamera = obj["ThirdPerson"].toBool();
    entityToLookAt = obj["EntityToLookAt"].toInt();
}

QJsonObject TransformComponent::toJson() const
{
    QJsonObject obj;

    QJsonArray children;
    for (auto& c : mChildren)
        children.push_back(static_cast<int>(c));
    obj["Children"] = children;
    obj["Parent"] = mParent;


    QJsonArray pos;
    pos.push_back(static_cast<double>(position.x));
    pos.push_back(static_cast<double>(position.y));
    pos.push_back(static_cast<double>(position.z));
    obj["Position"] = pos;

    QJsonArray rot;
    rot.push_back(static_cast<double>(rotation.w));
    rot.push_back(static_cast<double>(rotation.x));
    rot.push_back(static_cast<double>(rotation.y));
    rot.push_back(static_cast<double>(rotation.z));
    obj["Rotation"] = rot;

    QJsonArray s;
    s.push_back(static_cast<double>(scale.x));
    s.push_back(static_cast<double>(scale.y));
    s.push_back(static_cast<double>(scale.z));
    obj["Scale"] = s;
    return obj;
}

void TransformComponent::fromJson(QJsonObject obj)
{
    mParent = obj["Parent"].toInt();

    QJsonArray children = obj["Children"].toArray();
    for(auto c : children)
    {
        mChildren.push_back(c.toInt());
    }

    QJsonArray pos = obj["Position"].toArray();
    position.x = static_cast<float>(pos[0].toDouble());
    position.y = static_cast<float>(pos[1].toDouble());
    position.z = static_cast<float>(pos[2].toDouble());

    QJsonArray rot = obj["Rotation"].toArray();
    rotation.w = static_cast<float>(rot[0].toDouble());
    rotation.x = static_cast<float>(rot[1].toDouble());
    rotation.y = static_cast<float>(rot[2].toDouble());
    rotation.z = static_cast<float>(rot[3].toDouble());

    QJsonArray s = obj["Scale"].toArray();
    scale.x = static_cast<float>(s[0].toDouble());
    scale.y = static_cast<float>(s[1].toDouble());
    scale.z = static_cast<float>(s[2].toDouble());
}

QJsonObject ScriptComponent::toJson() const
{
    QJsonObject obj;
    obj.insert("FileName", mFileName);
    return obj;
}

void ScriptComponent::fromJson(QJsonObject obj)
{
    mFileName = obj["FileName"].toString();
}


QJsonObject CollisionComponent::toJson() const
{
    QJsonObject obj;
    QJsonArray o;
    o.push_back(static_cast<double>(localOrigin.x));
    o.push_back(static_cast<double>(localOrigin.y));
    o.push_back(static_cast<double>(localOrigin.z));
    obj["LocalOrigin"] = o;
    obj["Radius"] = static_cast<double>(radius);
    return obj;
}

void CollisionComponent::fromJson(QJsonObject obj)
{
    QJsonArray o = obj["LocalOrigin"].toArray();
    localOrigin.x = static_cast<float>(o[0].toDouble());
    localOrigin.y = static_cast<float>(o[1].toDouble());
    localOrigin.z = static_cast<float>(o[2].toDouble()) ;
    radius = static_cast<float>(obj["Radius"].toDouble());
}

QJsonObject AudioComponent::toJson() const
{
    QJsonObject obj;
    obj["FileName"] = filename;
    obj["Gain"] = static_cast<double>(mGain);
    obj["ShouldLoop"] = mShouldLoop;
    return obj;
}

void AudioComponent::fromJson(QJsonObject obj)
{
    filename = obj["FileName"].toString();


    mGain = static_cast<bool>(obj["Gain"].toDouble());
    mShouldLoop = obj["ShouldLoop"].toBool();
    mSoundSource = Engine::getInstance()->mSoundManager->createSource(
                filename.toStdString(),
                Engine::getInstance()->mTransformSystem->getWorldPosition(eID),
                gsl::assetFilePath + "Sounds/" + filename.toStdString(),
                mShouldLoop,
                mGain);
}

QJsonObject MaterialComponent::toJson() const
{
    QJsonObject obj;
    QJsonArray ambientColor;
    ambientColor.push_back(static_cast<double>(mAmbientColor.x));
    ambientColor.push_back(static_cast<double>(mAmbientColor.y));
    ambientColor.push_back(static_cast<double>(mAmbientColor.z));
    obj["AmbientColor"] = ambientColor;

    QJsonArray diffuseColor;
    diffuseColor.push_back(static_cast<double>(mDiffuseColor.x));
    diffuseColor.push_back(static_cast<double>(mDiffuseColor.y));
    diffuseColor.push_back(static_cast<double>(mDiffuseColor.z));
    obj["DiffuseColor"] = diffuseColor;

    QJsonArray specularColor;
    specularColor.push_back(static_cast<double>(mSpecularColor.x));
    specularColor.push_back(static_cast<double>(mSpecularColor.y));
    specularColor.push_back(static_cast<double>(mSpecularColor.z));
    obj["SpecularColor"] = specularColor;

    obj["Shininess"] = static_cast<int>(mShininess);

    obj["Filename"] = QString::fromStdString(mTextureFile);
    obj["Shader"] = Engine::getInstance()->mRenderSystem->findIndex(mShader);

    return obj;
}

void MaterialComponent::fromJson(QJsonObject obj)
{
    QJsonArray ambientColor = obj["AmbientColor"].toArray();
    mAmbientColor.x = static_cast<float>(ambientColor[0].toDouble());
    mAmbientColor.y = static_cast<float>(ambientColor[1].toDouble());
    mAmbientColor.z = static_cast<float>(ambientColor[2].toDouble());

    QJsonArray diffuseColor = obj["DiffuseColor"].toArray();
    mDiffuseColor.x = static_cast<float>(diffuseColor[0].toDouble());
    mDiffuseColor.y = static_cast<float>(diffuseColor[1].toDouble());
    mDiffuseColor.z = static_cast<float>(diffuseColor[2].toDouble());

    QJsonArray specularColor = obj["SpecularColor"].toArray();
    mSpecularColor.x = static_cast<float>(specularColor[0].toDouble());
    mSpecularColor.y = static_cast<float>(specularColor[1].toDouble());
    mSpecularColor.z = static_cast<float>(specularColor[2].toDouble());


    mShininess = static_cast<float>(obj["Shininess"].toDouble());

    mTextureFile = obj["Filename"].toString().toStdString();
    mTexture = Engine::getInstance()->mObjectManager->getTexture(mTextureFile);
    mShader = Engine::getInstance()->mRenderSystem->findShaderByIndex(obj["Shader"].toInt());
}

QJsonObject MeshComponent::toJson() const
{
    QJsonObject obj;

    obj["FileName"] = QString::fromStdString(fileName);
    obj["isVisible"] = bIsVisible;



    return obj;
}
void MeshComponent::fromJson(QJsonObject obj)
{
    fileName = obj["FileName"].toString().toStdString();
    mesh = Engine::getInstance()->mObjectManager->getMesh(fileName);
    bIsVisible = obj["isVisible"].toBool();
}

QJsonObject PhysicsComponent::toJson() const
{
    QJsonObject obj;
    obj["Speed"] = speed;
    return obj;
}

void PhysicsComponent::fromJson(QJsonObject obj)
{
    speed = obj["Speed"].toDouble();
}


QJsonObject InputComponent::toJson() const
{
    QJsonObject obj;
    QJsonArray input;
    input.push_back(static_cast<double>(inputVector.x));
    input.push_back(static_cast<double>(inputVector.y));
    input.push_back(static_cast<double>(inputVector.z));
    obj["Input"] = input;

    return obj;
}

void InputComponent::fromJson(QJsonObject obj)
{
    QJsonArray input = obj["Input"].toArray();
    inputVector.x = static_cast<float>(input[0].toDouble());
    inputVector.y = static_cast<float>(input[1].toDouble());
    inputVector.z = static_cast<float>(input[2].toDouble());

}

QJsonObject LandscapeComponent::toJson() const
{
    QJsonObject obj;

    obj["xmin"] = xmin;
    obj["xmax"] = xmax;
    obj["ymin"] = ymin;
    obj["ymax"] = ymax;
    obj["zmin"] = zmin;
    obj["zmax"] = zmax;
    obj["h"] = h;
    obj["Width"] = static_cast<int>(mWidth);
    obj["Height"] = static_cast<int>(mHeight);
    obj["FileName"] = filename;
    return obj;
}

void LandscapeComponent::fromJson(QJsonObject obj)
{
    filename = obj["FileName"].toString();
    qDebug() << "Reading landscape: " << filename;
    xmin = obj["xmin"].toDouble();
    xmax = obj["xmax"].toDouble();
    ymin = obj["ymin"].toDouble();
    ymax = obj["ymax"].toDouble();
    zmin = obj["zmin"].toDouble();
    zmax = obj["zmax"].toDouble();
    h = obj["h"].toDouble();
    mWidth = obj["Width"].toInt();
    mHeight = obj["Height"].toInt();


}

QJsonObject LightComponent::toJson() const
{

    QJsonObject obj;

    QJsonArray ambientCol;
    ambientCol.push_back(static_cast<double>(mAmbientColor.x));
    ambientCol.push_back(static_cast<double>(mAmbientColor.y));
    ambientCol.push_back(static_cast<double>(mAmbientColor.z));
    obj["AmbientColor"] = ambientCol;

    QJsonArray diffuseCol;
    diffuseCol.push_back(static_cast<double>(mDiffuseColor.x));
    diffuseCol.push_back(static_cast<double>(mDiffuseColor.y));
    diffuseCol.push_back(static_cast<double>(mDiffuseColor.z));
    obj["DiffuseColor"] = diffuseCol;

    QJsonArray specularCol;
    specularCol.push_back(static_cast<double>(mSpecularColor.x));
    specularCol.push_back(static_cast<double>(mSpecularColor.y));
    specularCol.push_back(static_cast<double>(mSpecularColor.z));
    obj["SpecularColor"] = specularCol;

//    obj["AmbientStrength"] = static_cast<double>(mAmbientStrength);
//    obj["LightStrength"] = static_cast<double>(mLightStrength);|
//    obj["SpecularStrength"] = static_cast<double>(mSpecularStrenght);
//    obj["SpecularExponent"] = static_cast<int>(mSpecularExponent);
    return obj;
}

void LightComponent::fromJson(QJsonObject obj)
{

    QJsonArray ambientCol = obj["AmbientColor"].toArray();
    mAmbientColor.x = static_cast<float>(ambientCol[0].toDouble());
    mAmbientColor.y = static_cast<float>(ambientCol[1].toDouble());
    mAmbientColor.z = static_cast<float>(ambientCol[2].toDouble());

    QJsonArray diffuseCol = obj["DiffuseColor"].toArray();
    mDiffuseColor.x = static_cast<float>(diffuseCol[0].toDouble());
    mDiffuseColor.y = static_cast<float>(diffuseCol[1].toDouble());
    mDiffuseColor.z = static_cast<float>(diffuseCol[2].toDouble());

    QJsonArray specularCol = obj["SpecularColor"].toArray();
    mSpecularColor.x = static_cast<float>(specularCol[0].toDouble());
    mSpecularColor.y = static_cast<float>(specularCol[1].toDouble());
    mSpecularColor.z = static_cast<float>(specularCol[2].toDouble());

//    mAmbientStrength = static_cast<float>(obj["AmbientStrength"].toDouble());
//    mLightStrength = static_cast<float>(obj["LightStrength"].toDouble());
//    mSpecularStrenght = static_cast<float>(obj["SpecularStrength"].toDouble());
//    mSpecularExponent = obj["SpecularExponent"].toInt();
}
