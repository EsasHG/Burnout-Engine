#include "Systems/inputsystem.h"

#include <QMouseEvent>
#include "Systems/engine.h"
#include "Systems/inputsystem.h"
#include "Systems/objectmanager.h"
#include "Systems/collisionsystem.h"
#include "Systems/transformsystem.h"
#include "Systems/physicssystem.h"
#include "Systems/landscapemanager.h"
#include "Rendering/mainwindow.h"
#include "Rendering/renderwindow.h"

#include "Components/audiocomponent.h"
#include "Components/inputcomponent.h"
#include "Components/meshcomponent.h"
#include "Components/physicscomponent.h"
#include "Components/landscapecomponent.h"
#include "Sound/soundsource.h"
#include "Systems/camerasystem.h"

InputSystem::InputSystem(MainWindow *inMainWindow) : mMainWindow(inMainWindow)
{

}

void InputSystem::sendMoveInput(unsigned inID, gsl::Vector3D inputVector)
{
    PhysicsComponent* p = Engine::getInstance()->mObjectManager->physicsComps[inID];
    if(p)
    {
        //send input to physics
        Engine::getInstance()->mPhysicsSystem->handleInput(inID, inputVector);
        //Engine::getInstance()->mTransformSystem->addPosition(inID, inputVector*p->speed);
    }
    else if(Engine::getInstance()->mObjectManager->transformComps[inID])
    {
        Engine::getInstance()->mTransformSystem->addPosition(inID, inputVector);
    }
}

void InputSystem::handleInput()
{
    if(mInput.LMB)
    {
        gsl::Vector3D ray_wor = findMousePickingRay();
        float id = Engine::getInstance()->mCollisionSystem->checkRaytraceCollison(
                    Engine::getInstance()->mTransformSystem->getWorldPosition(Engine::getInstance()->mCameraSystem->mCurrentCamera->eID),
                    Engine::getInstance()->mTransformSystem->getWorldPosition(Engine::getInstance()->mCameraSystem->mCurrentCamera->eID)+ (ray_wor * 1000)
                    );
        if(id>=0)
          mMainWindow->selectItem(id);

        std::shared_ptr<LandscapeManager> landscape = Engine::getInstance()->mLandscapeManager;
        for(auto& l : Engine::getInstance()->mObjectManager->landscapeComps)
        {
            if(!l) continue;
            MeshComponent* m = Engine::getInstance()->mObjectManager->meshComps[l->eID];
            if(!m) continue;
            gsl::Vector3D collision = Engine::getInstance()->mCollisionSystem->rayMeshCollision(
                        Engine::getInstance()->mTransformSystem->getWorldPosition(Engine::getInstance()->mCameraSystem->mCurrentCamera->eID),
                        (ray_wor),
                        m
                        );
            if(landscape->isSculptingAllowed)
            {
                if(mInput.ALT)
                {
                    landscape->sculpt(l,collision, false);
                }
                else
                {
                    landscape->sculpt(l,collision);
                }
            }

        }
    }
    if(mInput.RMB)
    {
        if(mInput.W)
            Engine::getInstance()->mCameraSystem->mCurrentCamera->mSpeed = Engine::getInstance()->mCameraSystem->mCameraSpeed;
        if(mInput.S)
            Engine::getInstance()->mCameraSystem->mCurrentCamera->mSpeed = -Engine::getInstance()->mCameraSystem->mCameraSpeed;
        if(mInput.D)
            Engine::getInstance()->mCameraSystem->moveRight(Engine::getInstance()->mCameraSystem->mCameraSpeed);
        if(mInput.A)
            Engine::getInstance()->mCameraSystem->moveRight(-Engine::getInstance()->mCameraSystem->mCameraSpeed);
        if(mInput.Q)
            Engine::getInstance()->mCameraSystem->updateHeigth(-Engine::getInstance()->mCameraSystem->mCameraSpeed);
        if(mInput.E)
            Engine::getInstance()->mCameraSystem->updateHeigth(Engine::getInstance()->mCameraSystem->mCameraSpeed);
    }
    else
    {
        if(Engine::bIsGameRunning && possessedEntity>=0)
        {
            unsigned eID = static_cast<unsigned>(possessedEntity);
            InputComponent* inputComp = Engine::getInstance()->mObjectManager->inputComps[eID];
            if(inputComp)
            {
                float speed = 0.5f;
                Engine::getInstance()->mObjectManager->inputComps[eID]->inputVector = gsl::Vector3D(0,0,0);
                if(mInput.W)
                    inputComp->inputVector += gsl::Vector3D(0,0,-speed);
                if(mInput.S)
                    inputComp->inputVector += gsl::Vector3D(0,0,speed);
                if(mInput.D)
                    inputComp->inputVector += gsl::Vector3D(speed,0,0);
                if(mInput.A)
                    inputComp->inputVector += gsl::Vector3D(-speed,0,0);
                if(mInput.Q)
                    inputComp->inputVector += gsl::Vector3D(0,speed,0);
                if(mInput.E)
                    inputComp->inputVector += gsl::Vector3D(0,-speed,0);
                sendMoveInput(eID, inputComp->inputVector);
            }
        }
    }
}

gsl::Vector3D InputSystem::findMousePickingRay()
{
    float x = (2.0f * mInput.MOUSEX) / mMainWindow->mRenderWindow->width() - 1.0f;
    float y = 1.0f - (2.0f * mInput.MOUSEY) / mMainWindow->mRenderWindow->height();

    gsl::Vector4D ray_clip(x, y, -1.0, 1.0);
    Engine::getInstance()->mCameraSystem->mCurrentCamera->mProjectionMatrix.inverse();
    gsl::Vector4D ray_eye = Engine::getInstance()->mCameraSystem->mCurrentCamera->mProjectionMatrix * ray_clip;
    Engine::getInstance()->mCameraSystem->mCurrentCamera->mProjectionMatrix.inverse();
    ray_eye = gsl::Vector4D(ray_eye.x, ray_eye.y, -1, 0);
    Engine::getInstance()->mCameraSystem->mCurrentCamera->mViewMatrix.inverse();
    gsl::Vector4D ray_wor4D(Engine::getInstance()->mCameraSystem->mCurrentCamera->mViewMatrix * ray_eye);
    gsl::Vector3D ray_wor(ray_wor4D.x, ray_wor4D.y, ray_wor4D.z);
    Engine::getInstance()->mCameraSystem->mCurrentCamera->mViewMatrix.inverse();
    ray_wor.normalize();
    return ray_wor;
}

void InputSystem::mouseMoveEvent(QMouseEvent *event)
{
    if (mInput.RMB)
    {
        float deltaX = event->pos().x() - mInput.MOUSEX;
        float deltaY = event->pos().y() - mInput.MOUSEY;

        if (deltaX != 0)
            Engine::getInstance()->mCameraSystem->yaw(
                        Engine::getInstance()->mCameraSystem->mCurrentCamera->sensitivity * deltaX);
        if (deltaY != 0)
            Engine::getInstance()->mCameraSystem->pitch(
                        Engine::getInstance()->mCameraSystem->mCurrentCamera->sensitivity * deltaY);
    }
    mInput.MOUSEX = event->x();
    mInput.MOUSEY = event->y();
}

void InputSystem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        mInput.RMB = true;
    if (event->button() == Qt::LeftButton)
    {
        mInput.LMB = true;
        mInput.MOUSEX = event->x();
        mInput.MOUSEY = event->y();
    }
    if (event->button() == Qt::MiddleButton)
        mInput.MMB = true;
}

void InputSystem::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        mInput.RMB = false;
    if (event->button() == Qt::LeftButton)
    {
        mInput.LMB = false;
        mInput.MOUSEX = event->x();
        mInput.MOUSEY = event->y();
    }
    if (event->button() == Qt::MiddleButton)
        mInput.MMB = false;
}

void InputSystem::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    //if RMB, change the speed of the camera
    if (mInput.RMB)
    {
        if (numDegrees.y() < 1)
            Engine::getInstance()->mCameraSystem->setCameraSpeed(0.01f);
        if (numDegrees.y() > 1)
            Engine::getInstance()->mCameraSystem-> setCameraSpeed(-0.01f);
    }
    event->accept();
}

void InputSystem::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) //Shuts down whole program
    {
        mMainWindow->close();
    }

    //    You get the keyboard input like this
    if(event->key() == Qt::Key_W)
    {
        mInput.W = true;
    }
    if(event->key() == Qt::Key_S)
    {
        mInput.S = true;
    }
    if(event->key() == Qt::Key_D)
    {
        mInput.D = true;
    }
    if(event->key() == Qt::Key_A)
    {
        mInput.A = true;
    }
    if(event->key() == Qt::Key_Q)
    {
        mInput.Q = true;
    }
    if(event->key() == Qt::Key_E)
    {
        mInput.E = true;
    }
    if(event->key() == Qt::Key_Z)
    {
    }
    if(event->key() == Qt::Key_X)
    {
    }
    if(event->key() == Qt::Key_Up)
    {
        mInput.UP = true;
    }
    if(event->key() == Qt::Key_Down)
    {
        mInput.DOWN = true;
    }
    if(event->key() == Qt::Key_Left)
    {
        mInput.LEFT = true;
    }
    if(event->key() == Qt::Key_Right)
    {
        mInput.RIGHT = true;
    }
    if(event->key() == Qt::Key_U)
    {
    }
    if(event->key() == Qt::Key_O)
    {
    }
    if(event->key() == Qt::Key_Alt)
    {
        mInput.ALT = true;
    }
}

void InputSystem::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
    {
        mInput.W = false;
    }
    if(event->key() == Qt::Key_S)
    {
        mInput.S = false;
    }
    if(event->key() == Qt::Key_D)
    {
        mInput.D = false;
    }
    if(event->key() == Qt::Key_A)
    {
        mInput.A = false;
    }
    if(event->key() == Qt::Key_Q)
    {
        mInput.Q = false;
    }
    if(event->key() == Qt::Key_E)
    {
        mInput.E = false;
    }
    if(event->key() == Qt::Key_Z)
    {
    }
    if(event->key() == Qt::Key_X)
    {
    }
    if(event->key() == Qt::Key_Up)
    {
        mInput.UP = false;
    }
    if(event->key() == Qt::Key_Down)
    {
        mInput.DOWN = false;
    }
    if(event->key() == Qt::Key_Left)
    {
        mInput.LEFT = false;
    }
    if(event->key() == Qt::Key_Right)
    {
        mInput.RIGHT = false;
    }
    if(event->key() == Qt::Key_U)
    {
    }
    if(event->key() == Qt::Key_O)
    {
    }
    if(event->key() == Qt::Key_Alt)
    {
        mInput.ALT = false;
    }
}
