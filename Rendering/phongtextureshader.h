#ifndef PHONGTEXTURESHADER_H
#define PHONGTEXTURESHADER_H
#include "Rendering/shader.h"

class LightComponent;
class Texture;

/** A Shader class that renders an entity with a texture, and depended on light.
    If the entity's material has no texture, a default white texture will be used instead.
    Only supports one directional light.
    Can also be used just as a phong shader by giving an object the default white texture.
 */
class PhongTextureShader : public Shader
{
public:
    PhongTextureShader(const std::string shaderName, const GLchar *geometryPath = nullptr);

    void transmitUniformData(gsl::Matrix4x4 *modelMatrix, MaterialComponent *material) override;
    /** Sets a new light to be used for the shading.
       @param light: the Lightcomponent that we use for the light.
     */
    void setLight(class LightComponent *light);
private:

    Texture* defaultTexture = nullptr;
    /// The shader currently only supports one light at a time. this is the light that will be used.
    LightComponent *mLight = nullptr;

    GLint mLAmbientColorUniform{-1};
    GLint mLSpecularColorUniform{-1};
    GLint mLDiffuseColorUniform{-1};
    GLint mLightDirectionUniform{-1};

    GLint mMAmbientColorUniform{-1};
    GLint mMDiffuseColorUniform{-1};
    GLint mMSpecularColorUniform{-1};
    GLint mMShininessUniform{-1};

    GLint textureUniform{-1};
    GLint mCameraPositionUniform{-1};



};

#endif // PHONGTEXTURESHADER_H
