#include "collisionsystem.h"

#include <cmath>

#include "engine.h"
#include "Systems/objectmanager.h"
#include "Systems/transformsystem.h"
#include "Systems/physicssystem.h"
#include "Systems/scriptsystem.h"
#include "Components/collisioncomponent.h"
#include "Components/meshcomponent.h"
#include "Components/landscapecomponent.h"
#include "Systems/landscapemanager.h"
CollisionSystem::CollisionSystem()
{

}

void CollisionSystem::
doCollisionDetection()
{
    std::shared_ptr<TransformSystem> tSys = Engine::getInstance()->mTransformSystem;
    //sjekker alle mot alle (ikke bra)
    unsigned numberOfCollisionComps = Engine::getInstance()->mObjectManager->collisionComps.size();
    for(unsigned i=0; i<numberOfCollisionComps; i++)
    {
        CollisionComponent* cComp = Engine::getInstance()->mObjectManager->collisionComps[i];
        if(!cComp) { continue;} // qDebug() << "NO COLLISION COMPONENT"))
        for (unsigned j=i+1;j<numberOfCollisionComps; ++j)
        {
            CollisionComponent* cCompJ = Engine::getInstance()->mObjectManager->collisionComps[j];
            if(!cCompJ) { continue;} // qDebug() << "NO COLLISION COMPONENT"))

            gsl::Vector3D iPos = tSys->getWorldPosition(i) + cComp->localOrigin;
            gsl::Vector3D jPos = tSys->getWorldPosition(j) + cCompJ->localOrigin;
            float distance = std::abs((iPos-jPos).length());
            if(distance < cComp->radius + cCompJ->radius)
            {
                ObjectHit hit(i, j);
                qDebug() << "Collision Detected: " << i << " and " << j;
                Engine::getInstance()->mScriptSystem->collision(hit);
            }
        }
    }
}

//send in a vector of objects to check?
void CollisionSystem::doCollisionDetectionWithLandscape()
{
    for(auto& landscape : Engine::getInstance()->mObjectManager->landscapeComps)
    {
        if(!landscape) continue;
        MeshComponent* meshComp = Engine::getInstance()->mObjectManager->meshComps[landscape->eID];
        std::vector<LandscapeHit> hits;
        for(auto& c : Engine::getInstance()->mObjectManager->collisionComps)
        {
            if(!c) {continue;}
            int trianglesLoopedThrough = 0;
            gsl::Vector3D playerPos = Engine::getInstance()->mTransformSystem->getWorldPosition(c->eID);
    //        gsl::Vector2D playerGridPos = gsl::Vector2D(playerPos.x/landscape->getH(), playerPos.z/landscape->getH());

    //        playerGridPos.x-= landscape->getMinX()/landscape->getH();
    //        playerGridPos.y-= landscape->getMinZ()/landscape->getH();

    //        unsigned playerTile = static_cast<unsigned>(playerGridPos.y)+ static_cast<unsigned>(playerGridPos.x)*landscape->getHeight();
    //        unsigned playerIndex = playerTile*6;
    //        qDebug() << "player grid pos: " << static_cast<int>(playerGridPos.x) << "," << static_cast<int>(playerGridPos.y) << "player Tile: " << playerTile << "player index: " << playerIndex;

            gsl::Vector2D temp = gsl::Vector2D(playerPos.x, playerPos.z);

            //for (unsigned int i=playerIndex; i<landscape->meshComp->mesh->mIndices.size(); i+=3)
            for (unsigned int i=0; i<meshComp->mesh->mIndices.size(); i+=3)
            {
                trianglesLoopedThrough++;
                gsl::Vector3D pos1;
                gsl::Vector3D pos2;
                gsl::Vector3D pos3;
                pos1 = meshComp->mesh->mVertices[meshComp->mesh->mIndices[i+2]].mXYZ;
                pos2 = meshComp->mesh->mVertices[meshComp->mesh->mIndices[i+1]].mXYZ;
                pos3 = meshComp->mesh->mVertices[meshComp->mesh->mIndices[i+0]].mXYZ;

                gsl::Vector3D bar = temp.barycentricCoordinates(gsl::Vector2D(pos1.x,pos1.z),gsl::Vector2D(pos2.x, pos2.z), gsl::Vector2D(pos3.x,pos3.z));

                if(bar.x>=0 && bar.x<=1 && bar.y>=0 && bar.y<=1 && bar.z>=0 && bar.z <=1)
                {
                    //qDebug() << "Found Triangle: " << bar;
                    gsl::Vector3D normal = gsl::Vector3D::cross(pos2 - pos1,pos3 - pos1);
                    normal.normalize();
                    gsl::Vector3D playerTempPos = gsl::Vector3D(pos1*bar.x + pos2*bar.y + pos3*bar.z);
                    float dist = gsl::Vector3D::dot((playerPos - playerTempPos),normal);
    //                qDebug() << "playertemppos: " << playerTempPos;
    //                qDebug() << "playerpos: " << playerPos;
    //                qDebug() << "distance to plane: " << dist;
                    if(dist < c->radius && dist > -c->radius)
                    {
                        hits.push_back(LandscapeHit{c->eID, landscape->eID,playerTempPos,normal, dist});
                    }
                    break;
                }
            }
            //qDebug() << "Triangles checked: " << trianglesLoopedThrough;
        }
        mLandscapeCollisions = hits;
    }
}

void CollisionSystem::handleLandscapeCollisions()
{
    for(auto& c : Engine::getInstance()->mObjectManager->collisionComps)
    {
        if(!c) {continue;}
        PhysicsComponent* p = Engine::getInstance()->mObjectManager->physicsComps[c->eID];

        bool hitFound = false;
        for(auto& h : mLandscapeCollisions)
        {
            if(c->eID == h.id1)
            {
                hitFound = true;
                //qDebug() << "hit found!";
                //if(p)
                //Engine::getInstance()->mPhysicsSystem->physicsLandscapeHit(h);
            }
        }
        if(!hitFound)
        {
            //qDebug() << "hit not found!";
             if(p)
            Engine::getInstance()->mPhysicsSystem->freeFall(c->eID);
        }

    }
}

//Möller–Trumbore intersection algorithm
gsl::Vector3D CollisionSystem::rayMeshCollision(gsl::Vector3D traceOrigin, gsl::Vector3D traceVec, MeshComponent *meshComp)
{
    if(!meshComp)
    {
        qDebug() << "CollisionSystem::rayMeshCollision: NO MESH";
        return gsl::Vector3D(0,0,0);
    }
    unsigned num = 0;
    bool isIndiced = false;
    if(meshComp->mesh->mIndices.size()>0)
    {
        isIndiced = true;
        num = meshComp->mesh->mIndices.size();
    }
    else
    {
        num = meshComp->mesh->mVertices.size();
    }
    std::vector<float> t_v;

    for (unsigned int i=0; i<num; i+=3)
    {
        gsl::Vector3D pos1;
        gsl::Vector3D pos2;
        gsl::Vector3D pos3;
        if(isIndiced)
        {
            pos1 = meshComp->mesh->mVertices[meshComp->mesh->mIndices[i+0]].mXYZ;
            pos2 = meshComp->mesh->mVertices[meshComp->mesh->mIndices[i+1]].mXYZ;
            pos3 = meshComp->mesh->mVertices[meshComp->mesh->mIndices[i+2]].mXYZ;
        }
        else
        {
            pos1 = meshComp->mesh->mVertices[i].mXYZ;
            pos2 = meshComp->mesh->mVertices[i+1].mXYZ;
            pos3 = meshComp->mesh->mVertices[i+2].mXYZ;
        }
        const float EPSILON = 0.00001;

        gsl::Vector3D edge1, edge2, h, s, q;
        float a,f,u,v;
        edge1 = pos2 - pos1;
        edge2 = pos3 - pos1;
        h = gsl::Vector3D::cross(traceVec,edge2);
        a = gsl::Vector3D::dot(edge1,h);
        if (a > -EPSILON && a < EPSILON)
            continue;    // This ray is parallel to this triangle.
        f = 1.0/a;
        s = traceOrigin - pos1;
        u = f * gsl::Vector3D::dot(s,h);
        if (u < 0.0 || u > 1.0)
            continue;
        q = gsl::Vector3D::cross(s,edge1);
        v = f * gsl::Vector3D::dot(traceVec,q);
        if (v < 0.0 || u + v > 1.0)
            continue;
        // At this stage we can compute t to find out where the intersection point is on the line.
        float t = f * gsl::Vector3D::dot(edge2,q);
        if (t > EPSILON && t < 1/EPSILON) // ray intersection
        {
            t_v.push_back(t);
        }
    }
    float T =9999999;
    for (auto t : t_v)
    {
        if(t < T)
            T = t;
    }
    return traceOrigin +traceVec * T;
}

//should maybe fix to return the closest one and not the one with the lowest index
int CollisionSystem::checkRaytraceCollison(gsl::Vector3D traceOrigin, gsl::Vector3D traceEnd)
{
    std::shared_ptr<TransformSystem> tSys = Engine::getInstance()->mTransformSystem;
    for(unsigned i=0; i<Engine::getInstance()->mObjectManager->collisionComps.size(); i++)
    {
        CollisionComponent* cComp = Engine::getInstance()->mObjectManager->collisionComps[i];
        if(!cComp) { continue;} // qDebug() << "NO COLLISION COMPONENT"))

        //http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
        gsl::Vector3D point = (tSys->getWorldPosition(i) + cComp->localOrigin);
        float distance = gsl::Vector3D::cross(point-traceOrigin, point-traceEnd).length() / (traceEnd-traceOrigin).length();
        if(distance <cComp->radius)
            return i;
            //qDebug() << "Collided! ID: " << i;
    }
    return -1;
}
