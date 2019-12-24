#include "engine.h"
#include "Sound/soundmanager.h"
#include "Systems/rendersystem.h"
#include "Rendering/mainwindow.h"
#include "transformsystem.h"
#include "Systems/collisionsystem.h"
#include "Systems/physicssystem.h"
#include "Systems/inputsystem.h"
#include "Systems/scriptsystem.h"
#include "Systems/camerasystem.h"
#include "Systems/scenemanager.h"
#include "Systems/landscapemanager.h"
#include "Systems/filemanager.h"
Engine* Engine::instance_ = NULL;
bool Engine::bIsGameRunning = false;

Engine::Engine()
{

}
Engine::~Engine()
{
}

void Engine::init(MainWindow* inMainWindow)
{
    mRenderSystem = std::make_shared<RenderSystem>();
    mSoundManager = std::make_shared<SoundManager>();
    mTransformSystem = std::make_shared<TransformSystem>();
    mCollisionSystem = std::make_shared<CollisionSystem>();
    mPhysicsSystem = std::make_shared<PhysicsSystem>();
    mScriptSystem = std::make_shared<ScriptSystem>();
    mCameraSystem = std::make_shared<CameraSystem>();
    mLandscapeManager = std::make_shared<LandscapeManager>();
    mSceneManager = std::make_shared<SceneManager>(inMainWindow);
    mObjectManager = std::make_shared<ObjectManager>(inMainWindow);
    mInputSystem = std::make_shared<InputSystem>(inMainWindow);
    mFileManager = std::make_shared<FileManager>();
    mSoundManager->init();
    mCameraSystem->init();
}

Engine* Engine::getInstance()
{
    if(!instance_)
        instance_ = new Engine();
    return instance_;
}
