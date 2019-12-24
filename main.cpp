#include <QApplication>
#include "Rendering/mainwindow.h"
/** \mainpage Welcome to the Burnout Engine documentation!
 * @date Fall of 2019.
 * This engine was created as a project in Game Engine Architecture at the Inland Norway University of Applied Sciences.
 * @author <a href="https://github.com/EsasHG/">EsasHG</a> on GitHub.
 */
int main(int argc, char *argv[])
{
    //Forces the usage of desktop OpenGL
    //Attribute must be set before Q(Gui)Application is constructed:
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    //Makes an Qt application
    QApplication a(argc, argv);

    //Makes the Qt MainWindow and shows it.
    MainWindow w;
    w.show();

    return a.exec();
}
