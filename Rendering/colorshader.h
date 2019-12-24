#ifndef COLORSHADER_H
#define COLORSHADER_H

#include "shader.h"

/** A Shader class for rendering an entity with its normals as the color.
 */
class ColorShader : public Shader
{
public:
    ColorShader(const std::string shaderName, const GLchar *geometryPath = nullptr);
    virtual ~ColorShader() override;
};

#endif // COLORSHADER_H
