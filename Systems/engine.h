#ifndef ENGINE_H
#define ENGINE_H
#include <memory>
#include <QFile>        //Reading from file

class RenderSystem;
class SoundManager;
class ObjectManager;
class TransformSystem;
class CollisionSystem;
class PhysicsSystem;
class InputSystem;
class ScriptSystem;
class CameraSystem;
class LandscapeManager;
class SceneManager;
class FileManager;

/** A singleton class that creates and holds all the systems and managers in the engine.
 */
class Engine
{
public:
    Engine();
    ~Engine();
    static Engine* getInstance();

    /** Creates all managers and systems, and then initializes them if needed.
     * @param inMainWindow: The main UI window. Is sent the systems that need it here
     */
    void init(class MainWindow* inMainWindow);

    std::shared_ptr<RenderSystem> mRenderSystem = nullptr;
    std::shared_ptr<SoundManager> mSoundManager;
    std::shared_ptr<ObjectManager> mObjectManager;
    std::shared_ptr<TransformSystem> mTransformSystem;
    std::shared_ptr<CollisionSystem> mCollisionSystem;
    std::shared_ptr<PhysicsSystem> mPhysicsSystem;
    std::shared_ptr<InputSystem> mInputSystem;
    std::shared_ptr<ScriptSystem> mScriptSystem;
    std::shared_ptr<CameraSystem> mCameraSystem;
    std::shared_ptr<LandscapeManager> mLandscapeManager;
    std::shared_ptr<SceneManager> mSceneManager;
    std::shared_ptr<FileManager> mFileManager;

    ///Static variable to check if the game is running.
    static bool bIsGameRunning;
private:
    static Engine* instance_;
};

#endif // ENGINE_H
