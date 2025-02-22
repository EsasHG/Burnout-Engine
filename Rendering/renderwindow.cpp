#include "innpch.h"
#include "renderwindow.h"
#include <QTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <chrono>
#include <QPainter>
#include <QString>
//#include <al.h>
//#include <alc.h>

#include "mainwindow.h"

#include "Systems/landscapemanager.h"
#include "colorshader.h"
#include "textureshader.h"
#include "Systems/engine.h"
#include "Systems/filemanager.h"
#include "Systems/objectmanager.h"
#include "Systems/collisionsystem.h"
#include "Systems/physicssystem.h"
#include "Systems/inputsystem.h"
#include "Systems/scriptsystem.h"
#include "Systems/camerasystem.h"
#include "Systems/transformsystem.h"
#include "Systems/scenemanager.h"
#include "Sound/soundmanager.h"
#include "Components/lightcomponent.h"
#include "Components/audiocomponent.h"
#include "GSL/quaternion.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)
{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }
    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);
}

RenderWindow::~RenderWindow()
{
    Engine::getInstance()->mSoundManager->cleanUp();
}

/// Sets up the general OpenGL stuff and the buffers needed to render a triangle

void RenderWindow::init()
{
    //Connect the gameloop timer to the render function:
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));

    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        qDebug() << "makeCurrent() failed";
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Print render version info:
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);    //enables depth sorting - must use GL_DEPTH_BUFFER_BIT in glClear
    glEnable(GL_CULL_FACE);     //draws only front side of models - usually what you want -
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //color used in glClear GL_COLOR_BUFFER_BIT

    Engine::getInstance()->init(mMainWindow);

    std::shared_ptr<RenderSystem> r = Engine::getInstance()->mRenderSystem;

    //********************** Making the objects to be drawn **********************

    //more sounds at once wrangle gain. fix?
    mObjectManager = Engine::getInstance()->mObjectManager;

    mObjectManager->spawnObject(OBJECTS::AXISOBJECT, r->mShaderProgram[0], "Axis");
    mObjectManager->spawnObject(OBJECTS::SKYBOX, r->mShaderProgram[1], "Skybox");
}

///Called each frame - doing the rendering
void RenderWindow::render()
{
    //calculate the time since last render-call
    //this should be the same as xxx in the mRenderTimer->start(xxx) set in RenderWindow::exposeEvent(...)
//    auto now = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<float> duration = now - mLastTime;
//    std::cout << "Chrono deltaTime " << duration.count()*1000 << " ms" << std::endl;
//    mLastTime = now;

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    bool bHasCamera = true;
    if(!Engine::getInstance()->mCameraSystem->mCurrentCamera)
        bHasCamera = false;
    gsl::Vector3D lastCameraPos(0,0,0);
    if(bHasCamera)
    {
        //Camera
        //TODO move to somewhere else)
        Engine::getInstance()->mCameraSystem->update();
        //This is so ugly oh my god
        Engine::getInstance()->mCameraSystem->mCurrentCamera->mSpeed = 0.f;  //cancel last frame movement

        //uses cameraPosition both before and after updating like this cause of the way camera movement is set up
        //this will need to be updated
        lastCameraPos = Engine::getInstance()->mTransformSystem->getWorldPosition(Engine::getInstance()->mCameraSystem->mCurrentCamera->eID);
        Engine::getInstance()->mInputSystem->handleInput();
        lastCameraPos = Engine::getInstance()->mTransformSystem->getWorldPosition(Engine::getInstance()->mCameraSystem->mCurrentCamera->eID) - lastCameraPos;
    }

    if(Engine::bIsGameRunning)
    {

        Engine::getInstance()->mScriptSystem->tick();
        Engine::getInstance()->mCollisionSystem->doCollisionDetection();
        Engine::getInstance()->mCollisionSystem->doCollisionDetectionWithLandscape();
        Engine::getInstance()->mCollisionSystem->handleLandscapeCollisions();
    }
//    Engine::getInstance()->mPhysicsSystem->runPhysics();

    //fix
    for(int i=0; i<mObjectManager->meshComps.size(); i++)
    {
        if(mObjectManager->meshComps[i])
        {
            Engine::getInstance()->mRenderSystem->draw(
                        mObjectManager->transformComps[i],
                        mObjectManager->meshComps[i],
                        mObjectManager->materials[i]
                        );
        }
    }
    if(bHasCamera)
    {
        Engine::getInstance()->mSoundManager->updateListener(
                    Engine::getInstance()->mTransformSystem->getWorldPosition(Engine::getInstance()->mCameraSystem->mCurrentCamera->eID),
                    lastCameraPos,                  //stores velocity in this variable so i dont have ot make a new one
                    Engine::getInstance()->mCameraSystem->mCurrentCamera->mForward,
                    Engine::getInstance()->mCameraSystem->mCurrentCamera->mUp);
    }


    //Calculate framerate before
    // checkForGLerrors() because that takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();

    //using our expanded OpenGL debugger to check if everything is OK.
//    checkForGLerrors();

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
//    auto start = std::chrono::high_resolution_clock::now();
    mContext->swapBuffers(this);
//    auto end = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<float> duration = end - start;
//    std::cout << "Chrono deltaTime " << duration.count()*1000 << " ms" << std::endl;

//    calculateFramerate();
    mObjectManager->deleteMarkedObjects();
}

//This function is called from Qt when window is exposed (shown)
//and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from

void RenderWindow::exposeEvent(QExposeEvent *)
{
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));


    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact..)
        mRenderTimer->start(1);
        mTimeStart.start();
    }

    Engine::getInstance()->mCameraSystem->setAspectRatio(static_cast<float>(width()) / height());

}

//Simple way to turn on/off wireframe mode
//Not totally accurate, but draws the objects with
//lines instead of filled polygons
void RenderWindow::toggleWireframe(bool checked)
{
    mWireframe = checked;
    if (mWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //turn on wireframe mode
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //turn off wireframe mode
        glEnable(GL_CULL_FACE);
    }
}

//The way this is set up is that we start the clock before doing the draw call,
//and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)    //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30) //once pr 30 frames = update the message twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000., 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 60 frames
        }
    }
}

/// Uses QOpenGLDebugLogger if this is present
/// Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            qDebug() << "glGetError returns " << err;
        }
    }
}

/// Tries to start the extended OpenGL debugger that comes with Qt
void RenderWindow::startOpenGLDebugger()
{

    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            qDebug() << "This system can not use QOpenGLDebugLogger, so we revert to glGetError()";

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                qDebug() << "Started OpenGL debug logger!";
        }

        if(mOpenGLDebugLogger)
            mOpenGLDebugLogger->disableMessages(QOpenGLDebugMessage::APISource, QOpenGLDebugMessage::OtherType, QOpenGLDebugMessage::NotificationSeverity);
    }
}

void RenderWindow::startGame()
{
    Engine::getInstance()->mSceneManager->saveScene(Engine::getInstance()->mSceneManager->sceneName);
    Engine::getInstance()->mCameraSystem->setGameCamera();

    Engine::getInstance()->mScriptSystem->beginPlay();
    Engine::bIsGameRunning = true;
}

void RenderWindow::stopGame()
{
    Engine::getInstance()->mCameraSystem->setEditorCamera();

    for(auto& s : Engine::getInstance()->mObjectManager->audioComps)
    {
        if(s && s->mSoundSource)
            s->mSoundSource->stop();
    }
    Engine::getInstance()->mScriptSystem->endPlay();
    Engine::bIsGameRunning = false;

    Engine::getInstance()->mSceneManager->saveScene(Engine::getInstance()->mSceneManager->sceneName);
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    Engine::getInstance()->mInputSystem->keyPressEvent(event);
}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    Engine::getInstance()->mInputSystem->keyReleaseEvent(event);
}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    Engine::getInstance()->mInputSystem->mousePressEvent(event);
}

void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Engine::getInstance()->mInputSystem->mouseReleaseEvent(event);
}

void RenderWindow::wheelEvent(QWheelEvent *event)
{
    Engine::getInstance()->mInputSystem->wheelEvent(event);
}

void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    Engine::getInstance()->mInputSystem->mouseMoveEvent(event);
}


