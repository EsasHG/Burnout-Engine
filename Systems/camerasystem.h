#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

class CameraComponent;

/** Responsible for managing the cameras in the scene.
 */
class CameraSystem
{
public:
    CameraSystem();

    /** Sets up game and editor cameras, and gives them frustum meshes.
     */
    void init();
    /** Moves the current camera right.
     * @param delta: the distance to move. Can be negative to move left.
     */
    void moveRight(float delta);
    /** Moves the current camera up.
     * @param deltaHeigth: the distance to move. Can be negative to move down.
     */
    void updateHeigth(float deltaHeigth);

    /** Updates the current camera's view matrix.
     */
    void update();

    /** Updates the current camera's forward vector.
     */
    void updateForwardVector();

    /** Updates the current camera's right vector.
     */
    void updateRightVector();

    /** Updates the current camera's yaw.
     */
    void yaw(float degrees);

    /** Updates the current camera's pitch.
     */
    void pitch(float degrees);

    /** Sets current camera to the one you send in.
     * @param inCamera: Camera to change to.
     */
    void setCamera(CameraComponent *inCamera);

    /** Sets camera to editor camera.
     */
    void setEditorCamera();

    /** Sets camera to game camera.
     */
    void setGameCamera();

    /** Sets camera speed, clamped between 0.01 and 0.5.
     * @param value: new camera speed.
     */
    void setCameraSpeed(float value);

    /** Sets current camera's aspect ratio.
     * @param inAspectRatio: new aspect ratio.
     */
    void setAspectRatio(float inAspectRatio);

    CameraComponent *mCurrentCamera{nullptr};

    /// Holds all the cameras we have. The first one must and will be the editor camera, and the second the game camera.
    std::vector<CameraComponent*> cameras;

    float mCameraSpeed = 0.1f;
private:
    /** Gives all existing cameras frustum meshes.
     */
    void setupFrustumMeshes();

    /** Updates current camera's projection matrix.
     */
    void updateProjectionMatrix();

    ///current aspect ratio. Stored so it can be given to a new camera when changed.
    float mAspectRatio{0.1f};

};

#endif // CAMERASYSTEM_H
