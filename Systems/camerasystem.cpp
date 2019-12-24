#include "camerasystem.h"
#include "Components/cameracomponent.h"
#include "Components/meshcomponent.h"
#include "Components/transformcomponent.h"
#include "Systems/engine.h"
#include "Systems/rendersystem.h"
#include "Systems/objectmanager.h"
#include "Systems/transformsystem.h"

CameraSystem::CameraSystem()
{

}

void CameraSystem::init()
{
    unsigned id = Engine::getInstance()->mObjectManager->spawnObject(OBJECTS::CAMERA,Engine::getInstance()->mRenderSystem->mShaderProgram[0], "Editor Camera");
    mCurrentCamera =Engine::getInstance()->mObjectManager->cameraComps[id];
    Engine::getInstance()->mTransformSystem->setPosition(mCurrentCamera->eID,gsl::Vector3D(0,1,1));
    mCameraSpeed = 0.1f;
    mCurrentCamera->bShowFrustum = false;
    Engine::getInstance()->mObjectManager->spawnObject(OBJECTS::CAMERA,Engine::getInstance()->mRenderSystem->mShaderProgram[0], "Game Camera");
    cameras[1]->bShowFrustum = true;
    //new system - shader sends uniforms so needs to get the view and projection matrixes from camera

    setupFrustumMeshes();
    setEditorCamera();

}

void CameraSystem::pitch(float degrees)
{
    //  rotate around mRight
    mCurrentCamera->mPitch -= degrees;
    //Engine::getInstance()->mTransformSystem->addRotation(mCurrentCamera->eID, gsl::Vector3D(-degrees,0,0));
    updateForwardVector();
}

void CameraSystem::yaw(float degrees)
{
    // rotate around mUp
    mCurrentCamera->mYaw -= degrees;
    //Engine::getInstance()->mTransformSystem->addRotation(mCurrentCamera->eID, gsl::Vector3D(0,-degrees,0));
    updateForwardVector();
}

void CameraSystem::updateRightVector()
{
    mCurrentCamera->mRight = mCurrentCamera->mForward^mCurrentCamera->mUp;
    mCurrentCamera->mRight.normalize();
}

void CameraSystem::updateForwardVector()
{
//    gsl::Vector3D rotation = Engine::getInstance()->mTransformSystem->getLocalRotation(mCurrentCamera->eID);

    //qDebug() << rotation;

    mCurrentCamera->mRight = gsl::Vector3D(1.f, 0.f, 0.f);
    //mCurrentCamera->mRight.rotateY(rotation.y);
    mCurrentCamera->mRight.rotateY(mCurrentCamera->mYaw);
    mCurrentCamera->mRight.normalize();
    mCurrentCamera->mUp = gsl::Vector3D(0.f, 1.f, 0.f);
    mCurrentCamera->mUp.rotateX(mCurrentCamera->mPitch);
    //mCurrentCamera->mUp.rotateX(rotation.x);
    mCurrentCamera->mUp.normalize();
    mCurrentCamera->mForward = mCurrentCamera->mUp^mCurrentCamera->mRight;

    updateRightVector();
}

void CameraSystem::update()
{
    if(!mCurrentCamera)
    {
        qDebug() << "No current camera!";
        return;
    }
    Engine::getInstance()->mTransformSystem->addPosition(mCurrentCamera->eID,mCurrentCamera->mForward * mCurrentCamera->mSpeed);
    gsl::Vector3D position = Engine::getInstance()->mTransformSystem->getWorldPosition(mCurrentCamera->eID);

    if(mCurrentCamera->bIsThirdPersonCamera)
    {
        gsl::Vector3D lookPos(0);
        if(mCurrentCamera->entityToLookAt >=0)
        {
            lookPos = Engine::getInstance()->mTransformSystem->getWorldPosition(mCurrentCamera->entityToLookAt);
            updateForwardVector();
        }

        mCurrentCamera->mViewMatrix.lookAt(
                    gsl::Vector3D(position),
                    lookPos,
                    gsl::Vector3D(0,1,0));
    }
    else
    {

        gsl::Matrix4x4 yawMat;
        gsl::Matrix4x4 pitchMat;

        pitchMat.rotateX(mCurrentCamera->mPitch);
        yawMat.rotateY(mCurrentCamera->mYaw);

        mCurrentCamera->mViewMatrix = pitchMat* yawMat;
        mCurrentCamera->mViewMatrix.translate(-position);

    }
}

void CameraSystem::updateHeigth(float deltaHeigth)
{
    Engine::getInstance()->mTransformSystem->addPosition(mCurrentCamera->eID,gsl::Vector3D(0,deltaHeigth,0));
}

void CameraSystem::moveRight(float delta)
{
    //This fixes a bug in the up and right calculations
    //so camera always holds its height when straifing
    //should be fixed thru correct right calculations!
    gsl::Vector3D right = mCurrentCamera->mRight;
    right.y = 0.f;
    Engine::getInstance()->mTransformSystem->addPosition(mCurrentCamera->eID, right*delta);
}

void CameraSystem::setGameCamera()
{
    //doesnt bother rendering game cam frustum while using it
    cameras[1]->bShowFrustum = false;
    setCamera(cameras[1]);
}

void CameraSystem::setEditorCamera()
{
    cameras[1]->bShowFrustum = true;
    setCamera(cameras[0]);
}

void CameraSystem::setCamera(CameraComponent *inCamera)
{
    mCurrentCamera = inCamera;
    Engine::getInstance()->mRenderSystem->mShaderProgram[0]->setCurrentCamera(inCamera);
    Engine::getInstance()->mRenderSystem->mShaderProgram[1]->setCurrentCamera(inCamera);
    Engine::getInstance()->mRenderSystem->mShaderProgram[2]->setCurrentCamera(inCamera);
    mCurrentCamera->mProjectionMatrix.perspective(45.f, mAspectRatio, 0.1f, 1000.f);
}

void CameraSystem::setCameraSpeed(float value)
{
    mCameraSpeed += value;

    //Keep within min and max values
    if(mCameraSpeed < 0.01f)
        mCameraSpeed = 0.01f;
    if (mCameraSpeed > 0.5f)
        mCameraSpeed = 0.5f;
}

void CameraSystem::setupFrustumMeshes()
{
    for(auto& c : cameras)
    {
        if(!c)
            continue;

        Engine::getInstance()->mObjectManager->addComponent(COMPONENTS::MATERIAL, c->eID);
        MeshComponent* meshComp = static_cast<MeshComponent*>(Engine::getInstance()->mObjectManager->addComponent(COMPONENTS::MESH, c->eID));
        meshComp->mesh = Engine::getInstance()->mObjectManager->getMesh("camerafrustum");
        meshComp->fileName = "camerafrustum";
    }
}

void CameraSystem::setAspectRatio(float inAspectRatio)
{
    mAspectRatio = inAspectRatio;
    updateProjectionMatrix();
}

void CameraSystem::updateProjectionMatrix()
{
    if(mCurrentCamera)
        mCurrentCamera->mProjectionMatrix.perspective(45.f, mAspectRatio, 0.1f, 1000.f);
}
