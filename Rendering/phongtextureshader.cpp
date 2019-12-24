#include "phongtextureshader.h"

#include "Components/materialcomponent.h"
#include "Components/lightcomponent.h"
#include "Components/cameracomponent.h"
#include "Systems/engine.h"
#include "Systems/objectmanager.h"
#include "Systems/transformsystem.h"
#include "Rendering/texture.h"

PhongTextureShader::PhongTextureShader(const std::string shaderName, const GLchar *geometryPath) : Shader(shaderName, geometryPath)
{

    mMatrixUniform = glGetUniformLocation( program, "mMatrix" );
    vMatrixUniform = glGetUniformLocation( program, "vMatrix" );
    pMatrixUniform = glGetUniformLocation( program, "pMatrix" );

    mLightDirectionUniform = glGetUniformLocation( program, "light.direction" );
    mLAmbientColorUniform = glGetUniformLocation( program, "light.ambientColor" );
    mLDiffuseColorUniform = glGetUniformLocation( program, "light.diffuseColor" );
    mLSpecularColorUniform = glGetUniformLocation( program, "light.specularColor" );

    mMAmbientColorUniform = glGetUniformLocation( program, "material.ambientColor" );
    mMDiffuseColorUniform = glGetUniformLocation( program, "material.diffuseColor" );
    mMSpecularColorUniform = glGetUniformLocation( program, "material.specularColor" );
    mMShininessUniform = glGetUniformLocation( program, "material.shininess" );

    textureUniform = glGetUniformLocation(program, "textureSampler");
    mCameraPositionUniform = glGetUniformLocation(program, "cameraPosition");
}


void PhongTextureShader::transmitUniformData(gsl::Matrix4x4 *modelMatrix, MaterialComponent *material)
{
    Shader::transmitUniformData(modelMatrix);

    //setting up default light in case of no light
    if(!mLight)
    {
        qDebug() << "No Light for phong texture shader! Using default light";
        unsigned lightID = Engine::getInstance()->mObjectManager->spawnObject(OBJECTS::LIGHT, Engine::getInstance()->mRenderSystem->mShaderProgram[0],"DefaultLight");
        qDebug() << lightID;
        glUniform3f(mLightDirectionUniform,0,0,0);
        glUniform3f(mLAmbientColorUniform, 0,0,0);
        glUniform3f(mLDiffuseColorUniform, 0,0,0);
        glUniform3f(mLSpecularColorUniform,0,0,0);
    }
    else
    {
        //    glUniform1i(textureUniform, material->mTextureUnit); //TextureUnit = 0 as default);
        gsl::Vector3D rot = Engine::getInstance()->mTransformSystem->getLocalRotation(mLight->eID);
        glUniform3f(mLightDirectionUniform, rot.x,rot.y,rot.z);
        glUniform3f(mLAmbientColorUniform,  mLight->mAmbientColor.x, mLight->mAmbientColor.y, mLight->mAmbientColor.z);
        glUniform3f(mLDiffuseColorUniform,  mLight->mDiffuseColor.x, mLight->mDiffuseColor.y, mLight->mDiffuseColor.z);
        glUniform3f(mLSpecularColorUniform, mLight->mSpecularColor.x, mLight->mSpecularColor.y, mLight->mSpecularColor.z);
    }

    glUniform3f(mMAmbientColorUniform,  material->mAmbientColor.x, material->mAmbientColor.y, material->mAmbientColor.z);
    glUniform3f(mMDiffuseColorUniform, material->mDiffuseColor.x, material->mDiffuseColor.y, material->mDiffuseColor.z);
    glUniform3f(mMSpecularColorUniform, material->mSpecularColor.x, material->mSpecularColor.y, material->mSpecularColor.z);

    glUniform1f(mMShininessUniform, material->mShininess);

    gsl::Vector3D camPos = Engine::getInstance()->mTransformSystem->getWorldPosition(mCurrentCamera->eID);
    glUniform3f(mCameraPositionUniform, camPos.x, camPos.y, camPos.z);

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
}

void PhongTextureShader::setLight(LightComponent *light)
{
    mLight = light;
}
