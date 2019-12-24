#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_4_1_Core>
#include "matrix4x4.h"

//#include "GL/glew.h" //We use QOpenGLFunctions instead, so no need for Glew (or GLAD)!

//This class is pretty much a copy of the shader class at
//https://github.com/SonarSystems/Modern-OpenGL-Tutorials/blob/master/%5BLIGHTING%5D/%5B8%5D%20Basic%20Lighting/Shader.h
//which is based on stuff from http://learnopengl.com/ and http://open.gl/.

//must inherit from QOpenGLFunctions_4_1_Core, since we use that instead of glfw/glew/glad

class CameraComponent;

/** Base class for all other Shader classes in the project. Seems to be unfit for rendering anything itself
 */
class Shader : protected QOpenGLFunctions_4_1_Core
{
public:
    // Constructor generates the shader on the fly
    Shader(const std::string shaderName, const GLchar *geometryPath = nullptr );
    virtual ~Shader();


    /** Use the current shader
     */
    void use();

    /** Get program number for this shader
     * @return program number
     */
    GLuint getProgram() const;

    /** Sends neccesary data to the shader.
       @param modelMatrix: Modelmatrix for the entity that we are sending the data for.
       @param material: The entity's material.
     */
    virtual void transmitUniformData(gsl::Matrix4x4 *modelMatrix, class MaterialComponent *material = nullptr);

    void setCurrentCamera(CameraComponent *currentCamera);

    CameraComponent *getCurrentCamera() const;

protected:
    GLuint program{0};
    GLint mMatrixUniform{-1};
    GLint vMatrixUniform{-1};
    GLint pMatrixUniform{-1};

    CameraComponent *mCurrentCamera{nullptr};

};

#endif
