#include "transformsystem.h"

#include "Components/transformcomponent.h"
#include "Components/audiocomponent.h"
#include "Components/cameracomponent.h"
#include "engine.h"
#include "Systems/objectmanager.h"
#include "Sound/soundsource.h"
#include "GSL/vector3d.h"
TransformSystem::TransformSystem()
{

}

void TransformSystem::setPosition(TransformComponent *tComp, gsl::Vector3D inPos)
{
    if(!tComp) {qDebug() << "not a valid transformcomponent";return;}
    tComp->position = inPos;

    AudioComponent* aComp = Engine::getInstance()->mObjectManager->audioComps[tComp->eID];
    if(aComp && aComp->mSoundSource)
        aComp->mSoundSource->setPosition(inPos);
}


void TransformSystem::setPosition(unsigned inID, gsl::Vector3D inPos)
{
    TransformComponent* tComp = Engine::getInstance()->mObjectManager->transformComps[inID];
    if(!tComp) {qDebug() << "not a valid transformcomponent";return;}
    tComp->position = inPos;

    AudioComponent* aComp = Engine::getInstance()->mObjectManager->audioComps[tComp->eID];
    if(aComp && aComp->mSoundSource)
        aComp->mSoundSource->setPosition(inPos);
}


void TransformSystem::setRotation(TransformComponent *tComp, gsl::Vector3D inRot)
{
    inRot.x = gsl::deg2radf(inRot.x);
    inRot.y = gsl::deg2radf(inRot.y);
    inRot.z = gsl::deg2radf(inRot.z);
    if(!tComp) {qDebug() << "not a valid transformcomponent";return;}
    tComp->rotation.fromEulerRotation(inRot);
}


void TransformSystem::setRotation(unsigned inID, gsl::Vector3D inRot)
{
    TransformComponent* tComp = Engine::getInstance()->mObjectManager->transformComps[inID];
    if(!tComp) {qDebug() << "not a valid transformcomponent";return;}

    inRot.x = gsl::deg2radf(inRot.x);
    inRot.y = gsl::deg2radf(inRot.y);
    inRot.z = gsl::deg2radf(inRot.z);

    tComp->rotation.fromEulerRotation(inRot);
}


void TransformSystem::setScale(TransformComponent *tComp, gsl::Vector3D inScale)
{
    if(!tComp) {qDebug() << "not a valid transformcomponent";return;}
    tComp->scale= inScale;
}


void TransformSystem::setScale(unsigned inID, gsl::Vector3D inScale)
{

    TransformComponent* tComp = Engine::getInstance()->mObjectManager->transformComps[inID];
    if(!tComp) {qDebug() << "not a valid transformcomponent";return;}
    tComp->scale= inScale;
}



gsl::Vector3D TransformSystem::getWorldPosition(unsigned inID)
{
    TransformComponent* tComp = Engine::getInstance()->mObjectManager->transformComps[inID];
    if(tComp)
    {
        if(tComp->mParent >=0)
            return  tComp->position + getWorldPosition(static_cast<unsigned>(tComp->mParent));
        else
            return  tComp->position;
    }
    else
        return gsl::Vector3D(0);
}


gsl::Vector3D TransformSystem::getWorldRotation(unsigned inID)
{
    TransformComponent* tComp = Engine::getInstance()->mObjectManager->transformComps[inID];
    if(tComp)
    {
        if(tComp->mParent >=0)
            return  tComp->rotation.toEulerRotationDeg() + getWorldRotation(static_cast<unsigned>(tComp->mParent));
        else
            return  tComp->rotation.toEulerRotationDeg();
    }
    else
        return gsl::Vector3D(0);
}


gsl::Vector3D TransformSystem::getWorldScale(unsigned inID)
{
    TransformComponent* tComp = Engine::getInstance()->mObjectManager->transformComps[inID];
    if(tComp)
    {
        if(tComp->mParent>=0)
            return  tComp->scale + getWorldScale(static_cast<unsigned>(tComp->mParent));
        else
            return  tComp->scale;
    }
    else
        return gsl::Vector3D(1);
}


gsl::Vector3D TransformSystem::getLocalPosition(unsigned inID)
{
     TransformComponent* tComp = Engine::getInstance()->mObjectManager->transformComps[inID];
     if(tComp)
         return tComp->position;
     else
         return  gsl::Vector3D(0);
}



gsl::Vector3D TransformSystem::getLocalRotation(unsigned inID)
{
    TransformComponent* tComp = Engine::getInstance()->mObjectManager->transformComps[inID];
    if(tComp)
        return tComp->rotation.toEulerRotationDeg();
    else
        return  gsl::Vector3D(0);
}


gsl::Vector3D TransformSystem::getLocalScale(unsigned inID)
{
    TransformComponent* tComp = Engine::getInstance()->mObjectManager->transformComps[inID];
    if(tComp)
        return tComp->scale;
    else
        return  gsl::Vector3D(1);
}

void TransformSystem::addPosition(TransformComponent *tComp, gsl::Vector3D inPos)
{
    if(!tComp) {qDebug() << "not a valid transformcomponent";return;}
    tComp->position+=inPos;

    AudioComponent* aComp = Engine::getInstance()->mObjectManager->audioComps[tComp->eID];
    if(aComp && aComp->mSoundSource)
        aComp->mSoundSource->setPosition(aComp->mSoundSource->getPosition()+=inPos);
}

void TransformSystem::addPosition(unsigned inID, gsl::Vector3D inPos)
{
    //CRASH: had a crash here when loading scene
    TransformComponent* tComp = Engine::getInstance()->mObjectManager->transformComps[inID];
    if(!tComp) {qDebug() << "not a valid transformcomponent";return;}
    tComp->position+=inPos;
//    for (auto& child : tComp->mChilds)
//        addPosition(child, inPos);

    AudioComponent* aComp = Engine::getInstance()->mObjectManager->audioComps[tComp->eID];
    if(aComp && aComp->mSoundSource)
        aComp->mSoundSource->setPosition(aComp->mSoundSource->getPosition()+=inPos);
}


void TransformSystem::addRotation(TransformComponent *tComp, gsl::Vector3D inRot)
{
    if(!tComp) {qDebug() << "not a valid transformcomponent";return;}
    inRot.x = gsl::deg2rad(inRot.x);
    inRot.y = gsl::deg2rad(inRot.y);
    inRot.z = gsl::deg2rad(inRot.z);
    gsl::Quaternion q;
    q.fromEulerRotation(inRot);
    tComp->rotation+= q;
}


void TransformSystem::addRotation(unsigned inID, gsl::Vector3D inRot)
{
    TransformComponent* tComp = Engine::getInstance()->mObjectManager->transformComps[inID];
    if(!tComp) {qDebug() << "not a valid transformcomponent";return;}
    inRot.x = gsl::deg2rad(inRot.x);
    inRot.y = gsl::deg2rad(inRot.y);
    inRot.z = gsl::deg2rad(inRot.z);
    gsl::Quaternion q;
    q.fromEulerRotation(inRot);
    tComp->rotation+=q;
}


void TransformSystem::addScale(TransformComponent *tComp, gsl::Vector3D inScale)
{
    if(!tComp) {qDebug() << "not a valid transformcomponent";return;}
    tComp->scale+=inScale;
}


void TransformSystem::addScale(unsigned inID, gsl::Vector3D inScale)
{
    TransformComponent* tComp = Engine::getInstance()->mObjectManager->transformComps[inID];
    if(!tComp) {qDebug() << "not a valid transformcomponent";return;}
    tComp->scale+=inScale;

}



gsl::Matrix4x4 TransformSystem::getTransformationMatrix(TransformComponent* transformComp)
{
    if(transformComp->mParent>=0)
    {
        return getTransformationMatrix(Engine::getInstance()->mObjectManager->transformComps[static_cast<unsigned>(transformComp->mParent)]) * getObjectTransformationMatrix(transformComp);
    }
    return getObjectTransformationMatrix(transformComp);
}

gsl::Matrix4x4 TransformSystem::getRotationMatrix(unsigned inID)
{
    TransformComponent* tComp = Engine::getInstance()->mObjectManager->transformComps[inID];
    if(!tComp) {qDebug() << "not a valid transformcomponent";return gsl::Matrix4x4();}
    return tComp->rotation.toMatrix();
}

gsl::Matrix4x4 TransformSystem::getObjectTransformationMatrix(TransformComponent* transformComp)
{
    gsl::Matrix4x4 posMat;
    posMat.setToIdentity();
    posMat.translate(transformComp->position);
    gsl::Matrix4x4 rotMat;
    transformComp->rotation.normalize();
    gsl::Vector3D temp = transformComp->rotation.toEulerRotationDeg();
    rotMat.rotateX(temp.x);
    rotMat.rotateY(temp.y);
    rotMat.rotateZ(temp.z);
    rotMat = transformComp->rotation.toMatrix();
    gsl::Matrix4x4 scaleMat;
    scaleMat.scale(transformComp->scale);
    return posMat*rotMat*scaleMat;
}
