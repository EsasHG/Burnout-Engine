#include "rendersystem.h"
#include "Components/meshcomponent.h"
#include "Components/transformcomponent.h"
#include "Components/materialcomponent.h"
#include "Components/cameracomponent.h"
#include "Systems/objectmanager.h"
#include "Systems/transformsystem.h"
#include "Systems/collisionsystem.h"
#include "Systems/camerasystem.h"
#include "Systems/engine.h"
#include "Rendering/texture.h"
#include "Rendering/colorshader.h"
#include "Rendering/textureshader.h"
#include "Rendering/phongtextureshader.h"

RenderSystem::RenderSystem()
{
    initializeOpenGLFunctions();

    mShaderProgram[0] = new ColorShader("plainshader");
    qDebug() << "Plain shader program id: " << mShaderProgram[0]->getProgram();
    mShaderProgram[1]= new TextureShader("textureshader");

    qDebug() << "Texture shader program id: " << mShaderProgram[1]->getProgram();

    mShaderProgram[2]= new PhongTextureShader("phongtextureshader");
    qDebug() << "Phong shader program id: " << mShaderProgram[2]->getProgram();


}

RenderSystem::~RenderSystem()
{
    for (int i = 0; i < 4; ++i) {
        if (mShaderProgram[i])
            delete mShaderProgram[i];
    }
}

void RenderSystem::reinitializeMesh(Mesh* mesh)
{
    //must call this to use OpenGL functions
    //initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glBindVertexArray( mesh->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glBindBuffer( GL_ARRAY_BUFFER, mesh->mVBO );

    glBufferData( GL_ARRAY_BUFFER, mesh->mVertices.size()*sizeof(Vertex), mesh->mVertices.data(), GL_STATIC_DRAW );

    glBindBuffer(GL_ARRAY_BUFFER, mesh->mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->mIndices.size() * sizeof(GLuint), mesh->mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void RenderSystem::initMesh(Mesh* mesh)
{
    //must call this to use OpenGL functions
    //initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mesh->mVAO );
    glBindVertexArray( mesh->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mesh->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mesh->mVBO );

    glBufferData( GL_ARRAY_BUFFER, mesh->mVertices.size()*sizeof(Vertex), mesh->mVertices.data(), GL_STATIC_DRAW );

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mesh->mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->mIndices.size() * sizeof(GLuint), mesh->mIndices.data(), GL_STATIC_DRAW);
}

void RenderSystem::draw(TransformComponent* transformComp,
            MeshComponent* meshComp,
            MaterialComponent* material
                )
{

    if(!meshComp->mesh || !meshComp->bIsVisible)
    return;
    if(!material)
    return;
    CameraComponent* c = Engine::getInstance()->mObjectManager->cameraComps[meshComp->eID];
    if(c && !c->bShowFrustum)
        return;

    gsl::Matrix4x4 tempMat;
    if(transformComp)
        tempMat = Engine::getInstance()->mTransformSystem->getTransformationMatrix(transformComp); //TODO send in an id instead?
    else
        tempMat.setToIdentity();

    //for frustum
    if(c)
    {
        gsl::Matrix4x4 posMat;
        posMat.setToIdentity();
        posMat.translate(Engine::getInstance()->mTransformSystem->getWorldPosition(c->eID));
        c->mProjectionMatrix.inverse();
        c->mViewMatrix.inverse();
        tempMat = posMat*c->mViewMatrix * c->mProjectionMatrix;
        c->mProjectionMatrix.inverse();
        c->mViewMatrix.inverse();

    }
    material->mShader->use();
    glBindVertexArray(meshComp->mesh->mVAO );
    material->mShader->transmitUniformData(&tempMat, material);
    if(meshComp->mesh->mIndices.empty())
    {
        switch (meshComp->mesh->renderType) {
        case RENDERTYPE::TRIANGLES:
            glDrawArrays(GL_TRIANGLES, 0, meshComp->mesh->mVertices.size());//mVertices.size());
            break;
        case RENDERTYPE::LINES:
            glDrawArrays(GL_LINES,0, meshComp->mesh->mVertices.size());
            break;
        case RENDERTYPE::TRIANGLE_STRIP:
            glDrawArrays(GL_TRIANGLE_STRIP,0, meshComp->mesh->mVertices.size());
            break;
        case RENDERTYPE::POINTS:
            glPointSize(10.f);
            glDrawArrays(GL_POINTS,0, meshComp->mesh->mVertices.size());
            break;
        }
    }
    else
    {
        switch (meshComp->mesh->renderType) {
        case RENDERTYPE::TRIANGLES:
            glDrawElements(GL_TRIANGLES, meshComp->mesh->mIndices.size(), GL_UNSIGNED_INT, nullptr);
            break;
        case RENDERTYPE::LINES:
            glDrawElements(GL_LINES, meshComp->mesh->mIndices.size(),GL_UNSIGNED_INT, nullptr);
            break;
        case RENDERTYPE::TRIANGLE_STRIP:
            glDrawElements(GL_TRIANGLE_STRIP, meshComp->mesh->mIndices.size(),GL_UNSIGNED_INT, nullptr);
            break;
        case RENDERTYPE::POINTS:
            glPointSize(10.f);
            glDrawArrays(GL_POINTS,0, meshComp->mesh->mVertices.size());
            break;
        }
    }
}

Shader *RenderSystem::findShaderByIndex(unsigned inIndex)
{
    if(inIndex >=4)
        return nullptr;
//    else if(inIndex == 3)
//        return mShaderProgram[2];
    for(int i=0; i<3; i++)
    {
        if(mShaderProgram[i] == mShaderProgram[inIndex])
            return mShaderProgram[i];
    }

    return nullptr;
}

int RenderSystem::findIndex(Shader *inShader)
{
    for(int i=0; i<4; i++)
    {
        if(mShaderProgram[i] == inShader)
            return i;
    }
    return -1;
}

void RenderSystem::changeLight(LightComponent *inNewLight)
{
    if(inNewLight)
    {
        static_cast<PhongTextureShader*>(mShaderProgram[2])->setLight(inNewLight);
    }
}
