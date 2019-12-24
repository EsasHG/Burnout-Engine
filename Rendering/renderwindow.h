#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QOpenGLWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <chrono>
#include <memory>
#include <QtGui>

#include "texture.h"
#include "Components/cameracomponent.h"
#include "input.h"

class QOpenGLContext;
class Shader;
class MainWindow;
class LightComponent;
class Triangle;
class SoundSource;


/// This inherits from QWindow to get access to the Qt functionality and
/// OpenGL surface.
/// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
/// This is the same as using glad and glw from general OpenGL tutorials

/** The window everything is rendered into.
 */
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    /** Initializes RenderWindow if it is uninutialized. Starts the render loop. Updates AspectRatio.
        Called every time the window is exposed or changes size.
     */
    void exposeEvent(QExposeEvent *) override;

//    /** Event for painting UI. Doesnt seem to work properly

//       @param event
//     */
//    void paintEvent(QPaintEvent *event) override;

    /** Sets wireframe on or off.

       @param checked: Mew wireframe state.
     */
    void toggleWireframe(bool checked);

    void checkForGLerrors();

    /** starts the game, plays all audio, runs beginPlay in scripts.
     */
    void startGame();

    /** stops the game and all audio, runs endPlay in scripts.
     */
    void stopGame();
    
private slots:
    /** The programs main loop.
        Called on a timer
     */
    void render();

private:
    /** Initializes RenderWindow, and other things in the program, like Engine.
        Spawns an axisObject and a skybox.
     */
    void init();

    QOpenGLContext *mContext{nullptr};
    bool mInitialized{false};

    //Light *mLight = nullptr;

    bool mWireframe{false};

    QTimer *mRenderTimer{nullptr};  //timer that drives the gameloop
    QElapsedTimer mTimeStart;       //time variable that reads the actual FPS

    MainWindow *mMainWindow{nullptr};    //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    void calculateFramerate();

    void startOpenGLDebugger();

    std::chrono::high_resolution_clock::time_point mLastTime;

    std::shared_ptr<class ObjectManager> mObjectManager;

protected:

    //The QWindow that we inherit from has these functions to capture mouse and keyboard.
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // RENDERWINDOW_H
