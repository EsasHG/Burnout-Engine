 #ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "input.h"

class MainWindow;
class CameraComponent;
class QMouseEvent;
class QWheelEvent;
class QKeyEvent;

/** Handles all the input for the engine.
   Events are sent here from RenderWindow.
 */
class InputSystem
{
public:
    InputSystem(MainWindow* inMainWindow);

    /** Handles input for the frame. Uses the state of mInput.
     */
    void handleInput();


    /** Updates mInput, and changes camera pitch and yaw if rmb is pressed.
       @param event: Event with input information
     */
    void mouseMoveEvent(QMouseEvent *event);

    /** Updates mInput when mouse is pressed.
       @param event: Event with input information.
     */
    void mousePressEvent(QMouseEvent *event);

    /** Updates mInput when mouse is released.
       @param event: Event with input information.
     */
    void mouseReleaseEvent(QMouseEvent *event);

    /** Changes camera speed.
       If rmb is pressed. Down is faster, up is slower.
       @param event: Event with input information.
     */
    void wheelEvent(QWheelEvent *event);

    /** Updates mInput when a key is released.
       @param event: Event with input information.
     */
    void keyReleaseEvent(QKeyEvent *event);

    /** Updates mInput when a key is pressed.
       @param event: Event with input information.
     */
    void keyPressEvent(QKeyEvent *event);

    /** redirects inputvector to either the physics system or directly to the transform system if the entity has no physics component
     * @param inID entity to handle input for
     * @param inputVector the input to handle
     */
    void sendMoveInput(unsigned inID, gsl::Vector3D inputVector);

    /// Index of the entity we are currently controlling when not controlling the camera.
    /// If below 0, no entity is controlled.
    int possessedEntity = -1;
    
private:

    /** Creates a ray from the camera in the direction of the mouse.
       @return The ray, as a vector3D.
     */
    gsl::Vector3D findMousePickingRay();

    /// A class with bools for different inputs.
    /// Set to true if the key is pressed, and false if not.
    /// Also stores the mouse position on screen, and the rotation of the mouse wheel.
    Input mInput;

    /// Pointer to the main window of the application.
    MainWindow* mMainWindow;
};

#endif // INPUTSYSTEM_H
