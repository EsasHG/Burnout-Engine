#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

#include "shader.h"

class Texture;

/** A Shader class that renders an entity with a texture.
    If the entity's material has no texture, a default white texture will be used instead.
 */
class TextureShader : public Shader
{
public:
    TextureShader(const std::string shaderName, const GLchar *geometryPath = nullptr);
    virtual ~TextureShader() override;

    void transmitUniformData(gsl::Matrix4x4 *modelMatrix, MaterialComponent *material) override;
    Texture* defaultTexture = nullptr;
private:
    GLint objectColorUniform{-1};
    GLint textureUniform{-1};
};

#endif // TEXTURESHADER_H
