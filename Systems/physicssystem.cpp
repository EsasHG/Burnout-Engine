#include "physicssystem.h"

#include "Systems/engine.h"
#include "Systems/collisionsystem.h"
#include "Systems/transformsystem.h"
#include "Systems/objectmanager.h"
#include "Systems/landscapemanager.h"
#include "Components/physicscomponent.h"
#include "Components/CollisionComponent.h"
#include "Components/meshcomponent.h"
#include "Components/landscapecomponent.h"
#include "GSL/math_constants.h"
PhysicsSystem::PhysicsSystem()
{

}

void PhysicsSystem::physicsLandscapeHit(LandscapeHit hit)
{
    CollisionComponent* c = Engine::getInstance()->mObjectManager->collisionComps[hit.id1];
    PhysicsComponent* p = Engine::getInstance()->mObjectManager->physicsComps[hit.id1];
    if(!c) {return;}
    if(!p) {return;}
    //qDebug() << "Hit start!";
    gsl::Vector3D N{0};
    //qDebug() << "Hit normal: " << hit.normal;
    N = hit.normal* gsl::Vector3D::dot(gsl::Vector3D(0, gsl::GRAVITY,0), hit.normal);
    //if(hit.clippingDistance > 0.5f)
    //Engine::getInstance()->mTransformSystem->addPosition(hit.hitID1,hit.hitNormal*hit.clippingDistance);

    if(hit.normal != p->surfaceNormal)
    {
        if(hit.normal == gsl::Vector3D(0)) //går til lufta
        {
            //qDebug() << "Hit Normal Is 0!";
        }
        else if(p->surfaceNormal== gsl::Vector3D(0))//kommer fra lufta
        {
            //qDebug() << "Surface Normal Is 0!";
            p->velocity = (gsl::Vector3D(0, -gsl::GRAVITY,0)+N).normalized() * gsl::Vector3D::dot(p->velocity, (gsl::Vector3D(0, -gsl::GRAVITY,0)+N).normalized());
        }
        else    //bytter trekant
        {
             //qDebug() << "Hit Normal: " << hit.hitNormal << "Surface Normal: " << p->surfaceNormal;
             gsl::Vector3D tempNormal = hit.normal + p->surfaceNormal;
             tempNormal.normalize();
             gsl::Vector3D tempVel = tempNormal*gsl::Vector3D::dot(p->velocity,tempNormal);
             tempVel= p->velocity - tempVel*2;
             p->velocity = tempVel;
        }
    }
    else
    {
        //qDebug() << "Normals Dont Differ!";
    }
    p->surfaceNormal = hit.normal;
    p->acceleration = gsl::Vector3D(hit.normal.x * hit.normal.y, hit.normal.y * hit.normal.y - 1 , hit.normal.z * hit.normal.y) * gsl::GRAVITY;
}

void PhysicsSystem::freeFall(unsigned inID)
{
    PhysicsComponent* p = Engine::getInstance()->mObjectManager->physicsComps[inID];
    p->surfaceNormal = gsl::Vector3D(0);
    p->acceleration = (gsl::Vector3D(0, -gsl::GRAVITY,0));
}

void PhysicsSystem::handleInput(unsigned inID, gsl::Vector3D inputVector)
{
    PhysicsComponent* p = Engine::getInstance()->mObjectManager->physicsComps[inID];
    CollisionComponent* c = Engine::getInstance()->mObjectManager->collisionComps[inID];
    TransformComponent* t = Engine::getInstance()->mObjectManager->transformComps[inID];
    if(!p || !t)
    {
        qDebug() << "No Physics or Transform Component!"; return;
    }
    std::shared_ptr<LandscapeManager> landscape = Engine::getInstance()->mLandscapeManager;
    if(!landscape )
    {
        qDebug() << "No Landscape!";
        Engine::getInstance()->mTransformSystem->addPosition(inID, inputVector*p->speed);
        return;
    }
    inputVector.normalize();

    gsl::Vector3D playerPos = Engine::getInstance()->mTransformSystem->getWorldPosition(inID)+inputVector*p->speed;
    gsl::Vector2D temp = gsl::Vector2D(playerPos.x, playerPos.z);

    //for (unsigned int i=playerIndex; i<landscape->meshComp->mesh->mIndices.size(); i+=3)
    for(auto& landscape : Engine::getInstance()->mObjectManager->landscapeComps)
    {
        if(!landscape) continue;
        MeshComponent* meshComp = Engine::getInstance()->mObjectManager->meshComps[landscape->eID];
        for (unsigned int i=0; i<meshComp->mesh->mIndices.size(); i+=3)
        {
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
    //                qDebug() << "playertemppos: " << playerTempPos;
    //                qDebug() << "playerpos: " << playerPos;
    //                qDebug() << "distance to plane: " << dist;
                //if(dist < c->radius && dist > -c->radius)
                {
                    Engine::getInstance()->mTransformSystem->setPosition(inID, playerTempPos+gsl::Vector3D(0,c->radius, 0));
                }
                break;
            }
        }

    }


}
