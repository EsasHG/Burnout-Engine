#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <string>

namespace gsl  //Game School Lib
{
    const std::string projectFolderName{"../INNgine2019/"};
    const std::string assetFilePath{projectFolderName + "Assets/"};
    const std::string shaderFilePath{projectFolderName + "Shaders/"};
    const std::string scriptFilePath{projectFolderName + "Scripts/"};
    const QString sceneFilePath{QString::fromStdString(projectFolderName )+ "Scenes/"};
}

#endif // CONSTANTS_H
