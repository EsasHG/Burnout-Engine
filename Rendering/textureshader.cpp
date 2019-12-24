#include "innpch.h"
#include "textureshader.h"
#include "Components/materialcomponent.h"
#include "Rendering/texture.h"
#include "Systems/engine.h"
#include "Systems/objectmanager.h"

TextureShader::TextureShader(const std::string shaderName, const GLchar *geometryPath)
    :Shader(shaderName, geometryPath)
{
    mMatrixUniform = glGetUniformLocation( program, "mMatrix" );
    vMatrixUniform = glGetUniformLocation( program, "vMatrix" );
    pMatrixUniform = glGetUniformLocation( program, "pMatrix" );
    objectColorUniform = glGetUniformLocation( program, "objectColor" );
    textureUniform = glGetUniformLocation(program, "textureSampler");

}

TextureShader::~TextureShader()
{
    qDebug() << "Deleting TextureShader";
}

void TextureShader::transmitUniformData(gsl::Matrix4x4 *modelMatrix, MaterialComponent *material)
{
    Shader::transmitUniformData(modelMatrix);
    if(!material || !material->mTexture)
    {
        if(!defaultTexture)
        {
            defaultTexture = Engine::getInstance()->mObjectManager->getTexture("white.bmp");
            qDebug() << "[TextureShader::transmitUniformData]: No texture for object! creating default texture!";
        }
        glUniform1i(textureUniform, defaultTexture->id());
    }
    else
        glUniform1i(textureUniform, material->mTexture->id()); //TextureUnit = 0 as default);

    glUniform3f(objectColorUniform, material->mDiffuseColor.x, material->mDiffuseColor.y, material->mDiffuseColor.z);
}
