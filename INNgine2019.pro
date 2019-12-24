QT          += core gui widgets
QT          += core qml

TEMPLATE    = app
CONFIG      += c++17

TARGET      = INNgine2019

PRECOMPILED_HEADER = innpch.h

INCLUDEPATH +=  ./GSL

mac {
    LIBS += -framework OpenAL
}

win32 {
    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
    LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a
}

HEADERS += \
    Components/audiocomponent.h \
    Components/basecomponent.h \
    Components/collisioncomponent.h \
    Components/inputcomponent.h \
    Components/landscapecomponent.h \
    Components/lightcomponent.h \
    Components/meshcomponent.h \
    Components/physicscomponent.h \
    Components/scriptcomponent.h \
    Components/transformcomponent.h \
    Components/materialcomponent.h \
    Components/cameracomponent.h \
    GSL/matrix2x2.h \
    GSL/matrix3x3.h \
    GSL/matrix4x4.h \
    GSL/quaternion.h \
    GSL/vector2d.h \
    GSL/vector3d.h \
    GSL/vector4d.h \
    GSL/gsl_math.h \
    GSL/math_constants.h \
    Rendering/phongtextureshader.h \
    Systems/camerasystem.h \
    Systems/collisionsystem.h \
    Systems/engine.h \
    Systems/filemanager.h \
    Systems/inputsystem.h \
    Systems/objectmanager.h \
    Systems/physicssystem.h \
    Systems/rendersystem.h \
    Systems/scenemanager.h \
    Systems/scriptsystem.h \
    Systems/transformsystem.h \
    Systems/landscapemanager.h \
    Sound/soundmanager.h \
    Sound/soundsource.h \
    Sound/wavfilehandler.h \
    Widgets/addcomponentwidget.h \
    Widgets/camerawidget.h \
    Widgets/landscapewidget.h \
    Widgets/lightwidget.h \
    Widgets/materialwidget.h \
    Widgets/meshwidget.h \
    Widgets/scriptwidget.h \
    Widgets/widgetbase.h \
    Widgets/physicswidget.h \
    constants.h \
    Rendering/renderwindow.h \
    Rendering/shader.h \
    Rendering/mainwindow.h \
    Rendering/texture.h \
    Widgets/soundwidget.h \
    Widgets/transformwidget.h \
    vertex.h \
    gltypes.h \
    input.h \
#    innpch.h \
    Rendering/colorshader.h \
    Rendering/textureshader.h \


SOURCES += main.cpp \
    GSL/matrix2x2.cpp \
    GSL/matrix3x3.cpp \
    GSL/matrix4x4.cpp \
    GSL/quaternion.cpp \
    GSL/vector2d.cpp \
    GSL/vector3d.cpp \
    GSL/vector4d.cpp \
    GSL/gsl_math.cpp \
    Rendering/phongtextureshader.cpp \
    Systems/camerasystem.cpp \
    Systems/collisionsystem.cpp \
    Systems/engine.cpp \
    Systems/filemanager.cpp \
    Systems/inputsystem.cpp \
    Systems/objectmanager.cpp \
    Systems/physicssystem.cpp \
    Systems/rendersystem.cpp \
    Systems/scenemanager.cpp \
    Systems/scriptsystem.cpp \
    Systems/transformsystem.cpp \
    Systems/landscapemanager.cpp \
    Sound/soundmanager.cpp \
    Sound/soundsource.cpp \
    Sound/wavfilehandler.cpp \
    Rendering/renderwindow.cpp \
    Rendering/mainwindow.cpp \
    Rendering/shader.cpp \
    Rendering/texture.cpp \
    Widgets/addcomponentwidget.cpp \
    Widgets/camerawidget.cpp \
    Widgets/landscapewidget.cpp \
    Widgets/lightwidget.cpp \
    Widgets/materialwidget.cpp \
    Widgets/meshwidget.cpp \
    Widgets/scriptwidget.cpp \
    Widgets/soundwidget.cpp \
    Widgets/transformwidget.cpp \
    Widgets/physicswidget.cpp \
    Widgets/widgetbase.cpp \
    vertex.cpp \
    input.cpp \
    Rendering/colorshader.cpp \
    Rendering/textureshader.cpp

FORMS += \
    Widgets/lightwidget.ui \
    Widgets/physicswidget.ui \
    Widgets/addcomponentwidget.ui \
    Widgets/camerawidget.ui \
    Widgets/landscapewidget.ui \
    Widgets/materialwidget.ui \
    Widgets/meshwidget.ui \
    Widgets/scriptwidget.ui \
    mainwindow.ui \
    Widgets/soundwidget.ui \
    Widgets/transformwidget.ui

DISTFILES += \
    Shaders/phongtextureshader.frag \
    Shaders/phongtextureshader.vert \
    Shaders/plainshader.frag \
    Shaders/plainshader.vert \
    Shaders/textureshader.frag \
    GSL/README.md \
    README.md \
    Shaders/textureshader.vert
