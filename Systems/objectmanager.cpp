#include "Systems/objectmanager.h"

#include <memory>

#include "Components/transformcomponent.h"
#include "Components/audiocomponent.h"
#include "Components/collisioncomponent.h"
#include "Components/physicscomponent.h"
#include "Components/scriptcomponent.h"
#include "Components/cameracomponent.h"
#include "Components/inputcomponent.h"
#include "Components/meshcomponent.h"
#include "Components/materialcomponent.h"
#include "Components/landscapecomponent.h"
#include "Systems/engine.h"
#include "Systems/transformsystem.h"
#include "Systems/filemanager.h"
#include "Systems/rendersystem.h"
#include "Systems/camerasystem.h"
#include "Systems/landscapemanager.h"
#include "Sound/soundmanager.h"
#include "Components/lightcomponent.h"
#include "Rendering/mainwindow.h"
#include "Rendering/texture.h"

ObjectManager::ObjectManager(MainWindow* inMainWindow) : mMainWindow(inMainWindow)
{
}

Mesh* ObjectManager::getMesh(std::string fileName)
{
    if(fileName == "")
    {
        qDebug() << "loading empty file!";
        return nullptr;
    }
    auto meshIt = meshMap.find(fileName);
    if(meshIt != meshMap.end())
    {
        return meshIt->second;
    }
    //load mesh
    return loadMesh(fileName);
}

Mesh* ObjectManager::loadMesh(std::string fileName)
{
    Mesh* m = nullptr;
    if(fileName.find(".obj") != std::string::npos)
    {
        m = new Mesh();
        std::pair<std::vector<Vertex>, std::vector<unsigned>> indiceVertices;
        indiceVertices = Engine::getInstance()->mFileManager->readOBJFile(fileName);
        m->mVertices = indiceVertices.first;
        m->mIndices = indiceVertices.second;
        m->renderType = RENDERTYPE::TRIANGLES;
    }
    else if(fileName.find(".txt") != std::string::npos)
    {
        m = new Mesh();
        m->renderType = RENDERTYPE::TRIANGLES;
        m->mVertices = Engine::getInstance()->mFileManager->readFile(fileName);
    }
    else if(fileName == "landscape")
    {
        m = new Mesh();
        m->renderType = RENDERTYPE::TRIANGLES;
    }
    else if(fileName == "skybox")
    {
        m = new Mesh();
        std::pair<std::vector<Vertex>, std::vector<unsigned>> indiceVertices;
        indiceVertices = getSkyboxVertices();
        m->mVertices = indiceVertices.first;
        m->mIndices = indiceVertices.second;
        m->renderType = RENDERTYPE::TRIANGLES;
    }
    else if(fileName == "camerafrustum")
    {
        m = new Mesh();
        std::pair<std::vector<Vertex>, std::vector<unsigned>> indiceVertices;
        indiceVertices = getCameraFrustum();
        m->mVertices = indiceVertices.first;
        m->mIndices = indiceVertices.second;
        m->renderType = RENDERTYPE::LINES;
    }
    else
    {
        qDebug() << "could not find mesh!";
        m = new Mesh();
        m->renderType = RENDERTYPE::TRIANGLES;
    }
    if(m)
    {
        meshMap.insert({fileName, m});
        Engine::getInstance()->mRenderSystem->initMesh(m);
    }
    return m;
}

Texture* ObjectManager::getTexture(std::string fileName)
{
    if(fileName == "")
    {
        qDebug() << "loading empty file!";
        return nullptr;
    }
    auto texIt = textureMap.find(fileName);
    if(texIt != textureMap.end())
    {
        return texIt->second;
    }
    //load mesh
    return loadTexture(fileName);
}

Texture* ObjectManager::loadTexture(std::string fileName)
{
    Texture* t  = nullptr;
    if(textureMap.empty())
        t = new Texture(fileName);
    else
        t = new Texture(fileName, textureMap.size());

    qDebug() << "ObjectManager::loadTexture: texure id: " << t->id() << "map size: " << textureMap.size();
    textureMap.insert({fileName, t});
    return t;
}

unsigned ObjectManager::spawnObject(OBJECTS OBJ, Shader* inShader, QString inName)
{
    mNames.push_back(inName);
    audioComps.push_back(nullptr);
    transformComps.push_back(nullptr);
    materials.push_back(nullptr);
    collisionComps.push_back(nullptr);
    physicsComps.push_back(nullptr);
    scriptComps.push_back(nullptr);
    cameraComps.push_back(nullptr);
    inputComps.push_back(nullptr);
    landscapeComps.push_back(nullptr);
    lightComps.push_back(nullptr);

    switch(OBJ)
    {
    case OBJECTS::CUBE:
        meshComps.push_back(new MeshComponent(idIncrementer));
        meshComps.back()->mesh = getMesh("box2.txt");
        meshComps.back()->mesh->renderType = RENDERTYPE::TRIANGLES;
        meshComps.back()->fileName = "box2.txt";
        materials.back() = new MaterialComponent(idIncrementer);
        materials.back()->mShader = inShader;
        break;
    case OBJECTS::BALL:
        meshComps.push_back(new MeshComponent(idIncrementer));
        meshComps.back()->mesh = getMesh("ball2.txt");
        meshComps.back()->fileName = "ball2.txt";
        meshComps.back()->mesh->renderType = RENDERTYPE::TRIANGLES;
        materials.back() = new MaterialComponent(idIncrementer);
        materials.back()->mShader = inShader;
        break;
    case OBJECTS::MONKEY:
        meshComps.push_back(new MeshComponent(idIncrementer));
        meshComps.back()->mesh = getMesh("monkey.obj");
        meshComps.back()->fileName = "monkey.obj";
        meshComps.back()->mesh->renderType = RENDERTYPE::TRIANGLES;
        materials.back() = new MaterialComponent(idIncrementer);
        materials.back()->mShader = inShader;
        break;
    case OBJECTS::AXISOBJECT:
        meshComps.push_back(new MeshComponent(idIncrementer));
        meshComps.back()->mesh = getMesh("axis.txt");
        meshComps.back()->fileName = "axis.txt";
        meshComps.back()->mesh->renderType = RENDERTYPE::LINES;
        materials.back() = new MaterialComponent(idIncrementer);
        materials.back()->mShader = inShader;
        //should maybe scale this up

        break;
    case OBJECTS::SKYBOX:
        meshComps.push_back(new MeshComponent(idIncrementer));
        meshComps.back()->mesh = getMesh("skybox");
        meshComps.back()->fileName = "skybox";
        meshComps.back()->mesh->renderType = RENDERTYPE::TRIANGLES;
        materials.back() = new MaterialComponent(idIncrementer);

        transformComps.back() = new TransformComponent(idIncrementer);
        Engine::getInstance()->mTransformSystem->setScale(transformComps.back(),gsl::Vector3D(45,45,45));
        materials.back()->mTexture = getTexture("skyboxSpace.bmp");
        materials.back()->mTextureFile= "skyboxSpace.bmp";
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, getTexture("skyboxSpace.bmp")->id());
        materials.back()->mShader = inShader;
        break;
    case OBJECTS::BILLBOARD:
        meshComps.push_back(new MeshComponent(idIncrementer));
        meshComps.back()->mesh = getMesh("Billboard.txt");
        meshComps.back()->fileName = "Billboard.txt";
        meshComps.back()->mesh->renderType = RENDERTYPE::TRIANGLE_STRIP;
        materials.back() = new MaterialComponent(idIncrementer);
        materials.back()->mTexture = getTexture("hund.bmp");
        materials.back()->mTextureFile= "hund.bmp";

        materials.back()->mShader = inShader;
        break;
    case OBJECTS::LIGHT:
        meshComps.push_back(new MeshComponent(idIncrementer));
        meshComps.back()->mesh = getMesh("light"); //TODO add light to getMesh
        meshComps.back()->fileName = "light";
        meshComps.back()->mesh->renderType = RENDERTYPE::TRIANGLES;
        materials.back() = new MaterialComponent(idIncrementer);
        materials.back()->mShader = inShader;
        lightComps.back() = new LightComponent(idIncrementer);
        Engine::getInstance()->mRenderSystem->changeLight(lightComps.back());
        transformComps.back() = new TransformComponent(idIncrementer);

        break;
    case OBJECTS::CAMERA:
        meshComps.push_back(nullptr);
        cameraComps.back() = new CameraComponent(idIncrementer);
        Engine::getInstance()->mCameraSystem->cameras.push_back(cameraComps.back());
        transformComps.back() = new TransformComponent(idIncrementer);

        break;
    case OBJECTS::ENTITY:
        meshComps.push_back(nullptr);
        break;
    }
    mMainWindow->addObjectToUI(mNames.back(), idIncrementer);
    idIncrementer++;
    return idIncrementer-1;
}

BaseComponent *ObjectManager::addComponent(COMPONENTS comp, unsigned inID)
{
    switch (comp)
    {
    case COMPONENTS::MESH:
        if(inID < meshComps.size() && meshComps[inID])
        {
            return meshComps[inID];
        }
        else if(inID < meshComps.size())
        {
            meshComps[inID] = new MeshComponent(inID);
            //if(!meshHolder[static_cast<int>(OBJECTS::CUBE)])
                qDebug() << "Mesh does not exist";
//            meshComps[inID]->mesh = getMesh("box2.txt");//meshHolder[static_cast<int>(OBJECTS::CUBE)];
//            meshComps[inID]->fileName = "box2.txt";//meshHolder[static_cast<int>(OBJECTS::CUBE)];
            return meshComps[inID];
        }
        else
        {
            qDebug() << "pushing back component";
        }
    case COMPONENTS::TRANSFORM:
        if(inID < transformComps.size() && transformComps[inID])
        {
            return transformComps[inID];
        }
        else if(inID < transformComps.size())
        {
            transformComps[inID] = new TransformComponent(inID);
            return transformComps[inID];
        }
        else
        {
            qDebug() << "pushing back component";
            transformComps.push_back(new TransformComponent(inID));
            return transformComps[inID];
        }
    case COMPONENTS::SOUND:
        if(inID < audioComps.size() && audioComps[inID])
        {
            return audioComps[inID];
        }
        else if(inID < audioComps.size())
        {
            audioComps[inID] = new AudioComponent(inID);
            return audioComps[inID];
        }
        else
        {
            qDebug() << "pushing back component";
            audioComps.push_back(new AudioComponent(inID));
            return audioComps[inID];
        }
    case COMPONENTS::COLLISION:
        if(inID < collisionComps.size() && collisionComps[inID])
        {
            return collisionComps[inID];
        }
        else if(inID < collisionComps.size())
        {
            collisionComps[inID] = new CollisionComponent(inID);
            return collisionComps[inID];
        }
        else
        {
            qDebug() << "pushing back component";
            collisionComps.push_back(new CollisionComponent(inID));
            return collisionComps[inID];
        }
    case COMPONENTS::MATERIAL:
        if(inID < materials.size() && materials[inID])
        {
            return materials[inID];
        }
        else if(inID < materials.size())
        {
            materials[inID] = new MaterialComponent(inID);
            materials[inID]->mShader = Engine::getInstance()->mRenderSystem->mShaderProgram[0];
            return materials[inID];
        }
        else
        {
            qDebug() << "pushing back component";
            materials.push_back(new MaterialComponent(inID));
            materials[inID]->mShader = Engine::getInstance()->mRenderSystem->mShaderProgram[0];
            return materials[inID];
        }
    case COMPONENTS::PHYSICS:
        if(inID < physicsComps.size() && physicsComps[inID])
        {
            return physicsComps[inID];
        }
        else if(inID < physicsComps.size())
        {
            physicsComps[inID] = new PhysicsComponent(inID);
            return physicsComps[inID];
        }
        else
        {
            qDebug() << "pushing back component";
            physicsComps.push_back(new PhysicsComponent(inID));
            return physicsComps[inID];
        }
    case COMPONENTS::SCRIPT:
        if(inID < scriptComps.size() && scriptComps[inID])
        {
            return scriptComps[inID];
        }
        else if(inID < scriptComps.size())
        {
            scriptComps[inID] = new ScriptComponent(inID);
            return scriptComps[inID];
        }
        else
        {
            qDebug() << "pushing back component";
            scriptComps.push_back(new ScriptComponent(inID));
            return scriptComps[inID];
        }
    case COMPONENTS::CAMERA:
        if(inID < cameraComps.size() && cameraComps[inID])
        {
            return cameraComps[inID];
        }
        else if(inID < cameraComps.size())
        {
            cameraComps[inID] = new CameraComponent(inID);
            Engine::getInstance()->mCameraSystem->cameras.push_back(cameraComps[inID]);
            return cameraComps[inID];
        }
        else
        {
            qDebug() << "pushing back component";
            cameraComps.push_back(new CameraComponent(inID));
            Engine::getInstance()->mCameraSystem->cameras.push_back(cameraComps.back());
            return cameraComps[inID];
        }
    case COMPONENTS::INPUT:
        if(inID < inputComps.size() && inputComps[inID])
        {
            return inputComps[inID];
        }
        else if(inID < inputComps.size())
        {
            inputComps[inID] = new InputComponent(inID);
            return inputComps[inID];
        }
        else
        {
            qDebug() << "pushing back component";
            inputComps.push_back(new InputComponent(inID));
            return inputComps[inID];
        }
    case COMPONENTS::LANDSCAPE:
        if(inID < landscapeComps.size() && landscapeComps[inID])
        {
            return landscapeComps[inID];
        }
        else if(inID < landscapeComps.size())
        {
            landscapeComps[inID] = new LandscapeComponent(inID);
            return landscapeComps[inID];
        }
        else
        {
            qDebug() << "pushing back component";
            landscapeComps.push_back(new LandscapeComponent(inID));
            return landscapeComps[inID];
        }
    case COMPONENTS::LIGHT:
        if(inID < lightComps.size() && lightComps[inID])
        {
            return lightComps[inID];
        }
        else if(inID < lightComps.size())
        {
            lightComps[inID] = new LightComponent(inID);
            Engine::getInstance()->mRenderSystem->changeLight(lightComps[inID]);
            return lightComps[inID];
        }
        else
        {
            qDebug() << "pushing back component";
            lightComps.push_back(new LightComponent(inID));
            Engine::getInstance()->mRenderSystem->changeLight(lightComps[inID]);
            return lightComps[inID];
        }
    }
    return nullptr;
}

void ObjectManager::addChild(unsigned parentID, unsigned childID)
{
    if(transformComps[parentID] && transformComps[childID])
    {
        transformComps[parentID]->mChildren.push_back(childID);
        transformComps[childID]->mParent = static_cast<int>(parentID);
    }
}

void ObjectManager::changeMesh(std::string inFileName, unsigned inID)
{

    meshComps[inID]->mesh = getMesh(inFileName);
    meshComps[inID]->fileName = inFileName;

}

void ObjectManager::removeAllChildren()
{
    for(auto& comp : transformComps)
    {
        if(comp)
        {
            for(unsigned c : comp->mChildren)
            {
                auto& childComp = Engine::getInstance()->mObjectManager->transformComps[c];
                if(childComp)
                    childComp->mParent = -1;
            }
            comp->mChildren.clear();
        }
    }
}

std::vector<BaseComponent *> ObjectManager::getAllComponentsWithID(unsigned inID)
{
    std::vector<BaseComponent *> components;
    components.push_back(meshComps[inID]);
    components.push_back(transformComps[inID]);
    components.push_back(audioComps[inID]);
    components.push_back(collisionComps[inID]);
    components.push_back(materials[inID]);
    components.push_back(physicsComps[inID]);
    components.push_back(scriptComps[inID]);
    components.push_back(cameraComps[inID]);
    components.push_back(inputComps[inID]);
    components.push_back(landscapeComps[inID]);
    components.push_back(lightComps[inID]);
    return components;
}

std::pair<std::vector<Vertex>, std::vector<unsigned int>> ObjectManager::getSkyboxVertices()
{
    std::pair<std::vector<Vertex>, std::vector<unsigned int>> mVertices;
        mVertices.first.insert( mVertices.first.end(),
        {//Vertex data for front
         Vertex{gsl::Vector3D(-1.f, -1.f, 1.f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(0.25f, 0.333f)},  //v0
         Vertex{gsl::Vector3D( 1.f, -1.f, 1.f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(0.5f,  0.333f)},  //v1
         Vertex{gsl::Vector3D(-1.f,  1.f, 1.f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(0.25f, 0.666f)},    //v2
         Vertex{gsl::Vector3D( 1.f,  1.f, 1.f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(0.5f,  0.666f)},    //v3

         //Vertex data for right
         Vertex{gsl::Vector3D(1.f, -1.f,  1.f),    gsl::Vector3D(1.f, 0.f, 0.f),   gsl::Vector2D(0.5f,  0.333f)},  //v4
         Vertex{gsl::Vector3D(1.f, -1.f, -1.f),    gsl::Vector3D(1.f, 0.f, 0.f),   gsl::Vector2D(0.75f, 0.333f)},  //v5
         Vertex{gsl::Vector3D(1.f,  1.f,  1.f),    gsl::Vector3D(1.f, 0.f, 0.f),   gsl::Vector2D(0.5f,  0.666f)},    //v6
         Vertex{gsl::Vector3D(1.f,  1.f, -1.f),    gsl::Vector3D(1.f, 0.f, 0.f),   gsl::Vector2D(0.75f, 0.666f)},    //v7

         //Vertex data for back
         Vertex{gsl::Vector3D( 1.f, -1.f, -1.f),   gsl::Vector3D(0.f, 0.f, -1.f),  gsl::Vector2D(0.75f, 0.333f)},  //v8
         Vertex{gsl::Vector3D(-1.f, -1.f, -1.f),   gsl::Vector3D(0.f, 0.f, -1.f),  gsl::Vector2D(1.f,   0.333f)},  //v9
         Vertex{gsl::Vector3D( 1.f,  1.f, -1.f),   gsl::Vector3D(0.f, 0.f, -1.f),  gsl::Vector2D(0.75f, 0.666f)},    //v10
         Vertex{gsl::Vector3D(-1.f,  1.f, -1.f),   gsl::Vector3D(0.f, 0.f, -1.f),  gsl::Vector2D(1.f,   0.666f)},    //v11

         //Vertex data for left
         Vertex{gsl::Vector3D(-1.f, -1.f, -1.f),   gsl::Vector3D(-1.f, 0.f, 0.f),  gsl::Vector2D(0.f, 0.333f)},    //v12
         Vertex{gsl::Vector3D(-1.f, -1.f,  1.f),   gsl::Vector3D(-1.f, 0.f, 0.f),  gsl::Vector2D(0.25f, 0.333f)},  //v13
         Vertex{gsl::Vector3D(-1.f,  1.f, -1.f),   gsl::Vector3D(-1.f, 0.f, 0.f),  gsl::Vector2D(0.f, 0.666f)},      //v14
         Vertex{gsl::Vector3D(-1.f,  1.f,  1.f),   gsl::Vector3D(-1.f, 0.f, 0.f),  gsl::Vector2D(0.25f,   0.666f)},  //v15

         //Vertex data for bottom
         Vertex{gsl::Vector3D(-1.f, -1.f, -1.f),   gsl::Vector3D(0.f, -1.f, 0.f),  gsl::Vector2D(0.25f, 0.f)},      //v16
         Vertex{gsl::Vector3D( 1.f, -1.f, -1.f),   gsl::Vector3D(0.f, -1.f, 0.f),  gsl::Vector2D(0.5f,  0.f)},      //v17
         Vertex{gsl::Vector3D(-1.f, -1.f,  1.f),   gsl::Vector3D(0.f, -1.f, 0.f),  gsl::Vector2D(0.25f, 0.333f)},   //v18
         Vertex{gsl::Vector3D( 1.f, -1.f,  1.f),   gsl::Vector3D(0.f, -1.f, 0.f),  gsl::Vector2D(0.5f,  0.333f)},   //v19

         //Vertex data for top
         Vertex{gsl::Vector3D(-1.f, 1.f,  1.f),    gsl::Vector3D(0.f, 1.f, 0.f),   gsl::Vector2D(0.25f, 0.666f)},    //v20
         Vertex{gsl::Vector3D( 1.f, 1.f,  1.f),    gsl::Vector3D(0.f, 1.f, 0.f),   gsl::Vector2D(0.5f,  0.666f)},    //v21
         Vertex{gsl::Vector3D(-1.f, 1.f, -1.f),    gsl::Vector3D(0.f, 1.f, 0.f),   gsl::Vector2D(0.25f, 0.999f)},      //v22
         Vertex{gsl::Vector3D( 1.f, 1.f, -1.f),    gsl::Vector3D(0.f, 1.f, 0.f),   gsl::Vector2D(0.5f,  0.999f)}       //v23
                          });

        mVertices.second.insert( mVertices.second.end(),
        { 0,  2,  1,  1,  2,  3,      //Face 0 - triangle strip (v0,  v1,  v2,  v3)
          4,  6,  5,  5,  6,  7,      //Face 1 - triangle strip (v4,  v5,  v6,  v7)
          8,  10,  9, 9, 10, 11,      //Face 2 - triangle strip (v8,  v9, v10,  v11)
          12, 14, 13, 13, 14, 15,     //Face 3 - triangle strip (v12, v13, v14, v15)
          16, 18, 17, 17, 18, 19,     //Face 4 - triangle strip (v16, v17, v18, v19)
          20, 22, 21, 21, 22, 23      //Face 5 - triangle strip (v20, v21, v22, v23)
                         });

        return mVertices;
}

std::pair<std::vector<Vertex>, std::vector<unsigned int>> ObjectManager::getLightVertices()
{
    std::pair<std::vector<Vertex>, std::vector<unsigned int>> mVertices;
        mVertices.first.insert( mVertices.first.end(),
             {//Vertex data - normals not correct
              Vertex{gsl::Vector3D(-0.5f, -0.5f,  0.5f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(0.f,  0.f)},   //Left low
              Vertex{gsl::Vector3D( 0.5f, -0.5f,  0.5f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(1.f,  0.f)},   //Right low
              Vertex{gsl::Vector3D( 0.0f,  0.5f,  0.0f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(0.5f, 0.5f)},  //Top
              Vertex{gsl::Vector3D( 0.0f, -0.5f, -0.5f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(0.5f, 0.5f)}   //Back low
        });

        mVertices.second.insert( mVertices.second.end(),
        {   0, 1, 2,
            1, 3, 2,
            3, 0, 2,
            0, 3, 1
                         });
        return mVertices;
}

std::pair<std::vector<Vertex>, std::vector<unsigned>> ObjectManager::getCameraFrustum()
{
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    float d = 1.f; //set to 1 to have it accurately
    float p = 0.95f; //set to 1 to have it accurately
    vertices.push_back(Vertex(gsl::Vector3D(-d,-d,-d),gsl::Vector3D(-1,0,0),gsl::Vector2D(0,0)));
    vertices.push_back(Vertex(gsl::Vector3D(d,-d,-d), gsl::Vector3D(1,0,0), gsl::Vector2D(0,0)));
    vertices.push_back(Vertex(gsl::Vector3D(d,-d,d),  gsl::Vector3D(1,0,0), gsl::Vector2D(0,0)));
    vertices.push_back(Vertex(gsl::Vector3D(-d,-d,d), gsl::Vector3D(-1,0,0),gsl::Vector2D(0,0)));
    vertices.push_back(Vertex(gsl::Vector3D(-d,d,-d), gsl::Vector3D(-1,0,0),gsl::Vector2D(0,0)));
    vertices.push_back(Vertex(gsl::Vector3D(d,d,-d),  gsl::Vector3D(1,0,0), gsl::Vector2D(0,0)));
    vertices.push_back(Vertex(gsl::Vector3D(d,d,d),   gsl::Vector3D(1,0,0), gsl::Vector2D(0,0)));
    vertices.push_back(Vertex(gsl::Vector3D(-d,d,d),  gsl::Vector3D(-1,0,0),gsl::Vector2D(0,0)));

    //middle thing


    vertices.push_back(Vertex(gsl::Vector3D(d,d,d*p),   gsl::Vector3D(1,0,0), gsl::Vector2D(0,0)));
    vertices.push_back(Vertex(gsl::Vector3D(d,-d,d*p),  gsl::Vector3D(1,0,0), gsl::Vector2D(0,0)));
    vertices.push_back(Vertex(gsl::Vector3D(-d,-d,d*p), gsl::Vector3D(-1,0,0),gsl::Vector2D(0,0)));
    vertices.push_back(Vertex(gsl::Vector3D(-d,d,d*p),  gsl::Vector3D(-1,0,0),gsl::Vector2D(0,0)));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(0);
    indices.push_back(4);
    indices.push_back(1);
    indices.push_back(5);
    indices.push_back(2);
    indices.push_back(6);
    indices.push_back(3);
    indices.push_back(7);
    indices.push_back(7);
    indices.push_back(6);
    indices.push_back(6);
    indices.push_back(5);
    indices.push_back(5);
    indices.push_back(4);
    indices.push_back(4);
    indices.push_back(7);

    //middle thing
    indices.push_back(8);
    indices.push_back(9);
    indices.push_back(9);
    indices.push_back(10);
    indices.push_back(10);
    indices.push_back(11);
    indices.push_back(11);
    indices.push_back(8);
    std::pair<std::vector<Vertex>, std::vector<unsigned>> v;
    v.first = vertices;
    v.second = indices;
    return v;
}

QStringList ObjectManager::getAllMeshNames()
{
    QStringList strings;

    if (meshMap.empty())
        return QStringList();
    auto meshIt = meshMap.begin();
    while (meshIt != meshMap.end())
    {
        strings.push_back(QString::fromStdString(meshIt->first));
        meshIt++;
    }
    return strings;
}

QStringList ObjectManager::getAllTextureNames()
{
    QStringList strings;

    if (textureMap.empty())
        return QStringList();
    auto texIt = textureMap.begin();
    while (texIt != textureMap.end())
    {
        strings.push_back(QString::fromStdString(texIt->first));
        texIt++;
    }
    return strings;
}

QString ObjectManager::OBJTypeToString(OBJECTS OBJ)
{
    switch(OBJ)
    {
    case OBJECTS::CUBE:
        return "Cube";
    case OBJECTS::BALL:
        return "Ball";
    case OBJECTS::MONKEY:
        return  "Monkey";
    case OBJECTS::BILLBOARD:
        return  "Billboard";
    case OBJECTS::ENTITY:
        return  "None";
    case OBJECTS::AXISOBJECT:
        return  "Axis";
    case OBJECTS::SKYBOX:
        return  "Skybox";
    case OBJECTS::LIGHT:
        return  "Light";
    case OBJECTS::LENGTH:
        return QString(static_cast<int>(OBJECTS::LENGTH));
    }
}

void ObjectManager::markForDeletion(unsigned inID)
{
    if(!entitiesToDelete.empty())
    {
        for(auto i : entitiesToDelete)
        {
            //returns if entity is already marked for deletion.
            if(i == inID)
                return;
        }
    }
    entitiesToDelete.push_back(inID);
}

void ObjectManager::deleteMarkedObjects()
{
    if(!entitiesToDelete.empty())
    {

        for(unsigned id : entitiesToDelete)
        {
            //checks for parent child relationships and reparents if neccesary
            if(transformComps[id])
            {
                int parentID= -1;
                if(transformComps[id]->mParent >= 0)
                {
                    parentID = transformComps[id]->mParent;
                    std::vector<unsigned> tempChildren = transformComps[parentID]->mChildren;
                    transformComps[parentID]->mChildren.clear();
                    for (unsigned c : tempChildren)
                    {
                        if(c!=id)
                            transformComps[parentID]->mChildren.push_back(c);
                    }
                }
                for(auto child : transformComps[id]->mChildren)
                {
                    transformComps[child]->mParent = parentID;
                }
            }

            std::vector<BaseComponent*> comps = getAllComponentsWithID(id);
            for (unsigned i=0; i<static_cast<unsigned>(COMPONENTS::LENGTH); i++)
            {
                if(comps[i])
                {
                    BaseComponent* temp = comps[i];
                    comps[i] = nullptr;
                    if(temp)
                    {
                        delete temp;
                    }
                }
            }
            meshComps[id] = nullptr;
            transformComps[id] = nullptr;
            audioComps[id] = nullptr;
            collisionComps[id] = nullptr;
            physicsComps[id] = nullptr;
            scriptComps[id] = nullptr;
            cameraComps[id] = nullptr;
            inputComps[id] = nullptr;
            landscapeComps[id] = nullptr;
            lightComps[id] = nullptr;
            mNames[id] = "";
        }
        entitiesToDelete.clear();
        mMainWindow->resetTreeWidget();
    }

}

void ObjectManager::clearEntities()
{
    for(int i=0; i< idIncrementer; i++)
    {
        delete meshComps[i];
        delete transformComps[i];
        delete audioComps[i];
        delete collisionComps[i];
        delete physicsComps[i];
        delete scriptComps[i];
        delete cameraComps[i];
        delete inputComps[i];
        delete landscapeComps[i];
        delete lightComps[i];
    }
    meshComps.clear();
    transformComps.clear();
    audioComps.clear();
    collisionComps.clear();
    physicsComps.clear();
    scriptComps.clear();
    cameraComps.clear();
    inputComps.clear();
    landscapeComps.clear();
    lightComps.clear();
    mNames.clear();
    idIncrementer=0;


}
