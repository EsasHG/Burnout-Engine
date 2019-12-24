#include "innpch.h"
#include "Systems/landscapemanager.h"
#include "Systems/engine.h"
#include "Systems/objectmanager.h"
#include "Systems/filemanager.h"
#include "Systems/collisionsystem.h"
#include "Components/meshcomponent.h"
#include "Components/landscapecomponent.h"
#include "Components/materialcomponent.h"
LandscapeManager::LandscapeManager()
{
}

LandscapeManager::~LandscapeManager()
{
}

LandscapeComponent* LandscapeManager::makeLandscape()
{
    int landscapeNum = Engine::getInstance()->mLandscapeManager->numOfLandscapes++;
    qDebug() << "LandscapeWidget::on_pushButton_clicked: No landscape! making a new one!";
    unsigned eID = Engine::getInstance()->mObjectManager->spawnObject(
                OBJECTS::ENTITY,
                Engine::getInstance()->mRenderSystem->mShaderProgram[0],
                QString("Landscape "+ QString::number(landscapeNum))
            );
    MeshComponent* meshComp = static_cast<MeshComponent*>(Engine::getInstance()->mObjectManager->addComponent(COMPONENTS::MESH,eID));
    MaterialComponent* materialComp = static_cast<MaterialComponent*>(Engine::getInstance()->mObjectManager->addComponent(COMPONENTS::MATERIAL,eID));
    LandscapeComponent* lComp= static_cast<LandscapeComponent*>( Engine::getInstance()->mObjectManager->addComponent(COMPONENTS::LANDSCAPE,eID));
        meshComp->mesh = Engine::getInstance()->mObjectManager->getMesh(QString("landscape" + QString::number(landscapeNum)).toStdString()); //TODO add light to getMesh
        meshComp->fileName = QString("landscape" + QString::number(landscapeNum)).toStdString();
        meshComp->mesh->renderType = RENDERTYPE::TRIANGLES;
        materialComp->mShader = Engine::getInstance()->mRenderSystem->mShaderProgram[0];
        return lComp;
}

void LandscapeManager::construct(LandscapeComponent* lComp)
{
    if(!lComp)
    {
        qDebug() << "TriangleSurface::construct: Landscape has no landscape component!";
        return;
    }
    MeshComponent* meshComp = Engine::getInstance()->mObjectManager->meshComps[lComp->eID];
    if(!meshComp)
    {
        qDebug() << "TriangleSurface::construct: Landscape has no mesh component!";
        return;
    }

    std::vector<gsl::Vector3D> normals;
    std::vector<Vertex> vertices;

    gsl::Vector3D normal(0,1,0);
    gsl::Vector2D uv(0,0);
    lComp->mWidth = lComp->zmax/lComp->h-lComp->zmin/lComp->h;
    lComp->mHeight = lComp->xmax/lComp->h-lComp->xmin/lComp->h;
    for (auto x=lComp->xmin; x<lComp->xmax; x+=lComp->h)
    {
        uv.x = (x-lComp->xmin)/ (lComp->xmax-lComp->xmin);
        for (auto z=lComp->zmin; z<lComp->zmax; z+=lComp->h)
        {
            uv.y = 1 - (z-lComp->zmin)/ (lComp->zmax-lComp->zmin);
            gsl::Vector3D pos1 = gsl::Vector3D(x,0,z);
            vertices.push_back(Vertex(pos1,normal,uv));
        }
    }

    std::vector<unsigned> tempIndices;
    tempIndices = makeIndices(lComp);

    meshComp->mesh->mVertices = vertices;
    meshComp->mesh->mIndices= tempIndices;
    calculateNormals(lComp);
    Engine::getInstance()->mRenderSystem->reinitializeMesh(meshComp->mesh);

    qDebug() << "TriangleSurface::construct: Inited landscape!";
}

//does this work?
void LandscapeManager::calculateNormals(LandscapeComponent *lComp)
{
    MeshComponent* meshComp = Engine::getInstance()->mObjectManager->meshComps[lComp->eID];
    if(!meshComp)
    {
        qDebug() << "TriangleSurface::construct: Landscape has no mesh component!";
        return;
    }
    Mesh* mesh = meshComp->mesh;
    for (unsigned int i=0; i<mesh->mIndices.size(); i+=3)
    {
        auto pos1 = mesh->mVertices[mesh->mIndices[i+0]].mXYZ;
        auto pos2 = mesh->mVertices[mesh->mIndices[i+1]].mXYZ;
        auto pos3 = mesh->mVertices[mesh->mIndices[i+2]].mXYZ;

        auto normal = gsl::Vector3D::cross(pos2-pos1,pos3-pos1);
        normal.normalize();

        mesh->mVertices[mesh->mIndices[i+0]].set_normal(normal);
        mesh->mVertices[mesh->mIndices[i+1]].set_normal(normal);
        mesh->mVertices[mesh->mIndices[i+2]].set_normal(normal);

    }
    std::vector<gsl::Vector3D> tempNormals;

}

std::vector<unsigned> LandscapeManager::makeIndices(LandscapeComponent* lComp)
{
    std::vector<unsigned> tempIndices;

    for(int i=0; i<lComp->mWidth*lComp->mHeight;i++)
    {
        if(i >= (lComp->mWidth*lComp->mHeight)-lComp->mHeight) break;
        if(i>1 && ((i+1)%lComp->mWidth)== 0) continue;

        tempIndices.push_back(i+1);
        tempIndices.push_back(i+lComp->mHeight);
        tempIndices.push_back(i);
        tempIndices.push_back(i+1);
        tempIndices.push_back(i+lComp->mHeight+1);
        tempIndices.push_back(i+lComp->mHeight);
    }
    return tempIndices;
}

void LandscapeManager::makeNormals(LandscapeComponent* lComp)
{
    MeshComponent* meshComp = Engine::getInstance()->mObjectManager->meshComps[lComp->eID];
    if(!meshComp)
    {
        qDebug() << "TriangleSurface::construct: Landscape has no mesh component!";
        return;
    }
    Mesh* mesh = meshComp->mesh;
    for (int i=0;i<mesh->mVertices.size(); i++)
    {
        gsl::Vector3D normal = gsl::Vector3D(0,0,0);
        gsl::Vector3D p1 = mesh->mVertices[i].mXYZ;
        gsl::Vector3D p2, p3;
        //trekant 1
        if(((i%lComp->mWidth)-1)>=0 && i >= lComp->mWidth)
        {
            p2 = mesh->mVertices[i-1].mXYZ - p1 ;
            p3 = mesh->mVertices[i-lComp->mWidth].mXYZ -p1;
            normal+= p2^p3;
        }
        else
        {
            normal += gsl::Vector3D(0,1,0);
        }
        //trekant 2
        if(((i%lComp->mWidth)-1)>=0 && i >= lComp->mWidth)
        {
            p2 = mesh->mVertices[i-lComp->mWidth].mXYZ -p1;
            p3 = mesh->mVertices[i-lComp->mWidth+1].mXYZ -p1;
            normal+= p2^p3;
        }
        else
        {
            normal += gsl::Vector3D(0,1,0);
        }
        //trekant 3
        if(((i%lComp->mWidth)+1)<lComp->mWidth && i >= lComp->mWidth)
        {
            p2 = mesh->mVertices[i-lComp->mWidth+1].mXYZ -p1;
            p3 = mesh->mVertices[i+1].mXYZ -p1;
            normal+= p2^p3;
        }
        else
        {
            normal += gsl::Vector3D(0,1,0);
        }
        //trekant 4
        if(((i%lComp->mWidth)+1)<lComp->mWidth && i < mesh->mVertices.size()-lComp->mWidth)
        {
            p2 = mesh->mVertices[i+1].mXYZ -p1;
            p3 = mesh->mVertices[i+lComp->mWidth].mXYZ -p1;
            normal+= p2^p3;
        }
        else
        {
            normal += gsl::Vector3D(0,1,0);
        }
        //trekant 5
        if(((i%lComp->mWidth)+1)<lComp->mWidth && i < mesh->mVertices.size()-lComp->mWidth)
        {
            p2 = mesh->mVertices[i+lComp->mWidth].mXYZ -p1;
            p3 = mesh->mVertices[i+lComp->mWidth-1].mXYZ -p1;
            normal+= p2^p3;
        }
        else
        {
            normal += gsl::Vector3D(0,1,0);
        }
        //trekant 6
        if(((i%lComp->mWidth)-1)<lComp->mWidth && i < mesh->mVertices.size()-lComp->mWidth)
        {
            p2 = mesh->mVertices[i+lComp->mWidth-1].mXYZ -p1;
            p3 = mesh->mVertices[i-1].mXYZ -p1;
            normal+= p2^p3;
        }
        else
        {
            normal += gsl::Vector3D(0,1,0);
        }
        normal.normalize();
        mesh->mVertices[i].set_normal(normal.x,normal.y,normal.z);
    }
}

std::vector<std::pair<Vertex*, float>> LandscapeManager::findVerticesInRange(LandscapeComponent* lComp, gsl::Vector3D point)
{
    MeshComponent* meshComp = Engine::getInstance()->mObjectManager->meshComps[lComp->eID];
    if(!meshComp)
    {
        qDebug() << "TriangleSurface::construct: Landscape has no mesh component!";
    }

    std::vector<std::pair<Vertex*, float>> vertices;
    float distance = 999999999.f;
    for(auto& vertex : meshComp->mesh->mVertices)
    {
        float d = (vertex.mXYZ - point).length();
        if(d <= sculptRange)
        {
            std::pair<Vertex*, float> temp = {&vertex, d};
            vertices.push_back(temp);
            distance = d;
        }
    }
    return vertices;
}

void LandscapeManager::sculpt(LandscapeComponent* lComp,gsl::Vector3D collisionPoint, bool sculptUp)
{
    MeshComponent* meshComp = Engine::getInstance()->mObjectManager->meshComps[lComp->eID];
    if(!meshComp)
    {
        qDebug() << "TriangleSurface::construct: Landscape has no mesh component!";
        return;
    }

    //qDebug() << "Collision Point: " << collisionPoint;
    std::vector<std::pair<Vertex*, float>>  vertices = findVerticesInRange(lComp,collisionPoint);

    //TODO refactor this code so it doesnt appear in both sculptUp and sculptDown
    for( auto& v : vertices)
    {
        //normalizes distance to center.
        float temp = v.second/sculptRange;
        float sensitivityModifyer = (1-temp);// linear modifier. Could somehow make this modifiable
        if(sculptUp)
        {
            v.first->mXYZ += gsl::Vector3D(0,sculptSensitivity* sensitivityModifyer,0);
            if(v.first->mXYZ.y > lComp->ymax)
                lComp->ymax = v.first->mXYZ.y;
        }
        else
        {
            v.first->mXYZ += gsl::Vector3D(0,-sculptSensitivity* sensitivityModifyer,0);
            if(v.first->mXYZ.y < lComp->ymin)
                lComp->ymin = v.first->mXYZ.y;
        }
    }
    calculateNormals(lComp);
    Engine::getInstance()->mRenderSystem->reinitializeMesh(meshComp->mesh);
}

void LandscapeManager::loadLandscape(LandscapeComponent* landscape)
{
    MeshComponent* m = Engine::getInstance()->mObjectManager->meshComps[landscape->eID];
    m->mesh->mVertices = Engine::getInstance()->mFileManager->readHeightMap(landscape->filename,landscape);
    m->mesh->mIndices = makeIndices(landscape);
    calculateNormals(landscape);
    Engine::getInstance()->mRenderSystem->reinitializeMesh(m->mesh);
}

