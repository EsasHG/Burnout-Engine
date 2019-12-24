#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "innpch.h"
/** Generated when an object hits the landscape. Holds collision info.
 */
struct LandscapeHit
{
    unsigned id1;
    unsigned id2;

    gsl::Vector3D position;
    gsl::Vector3D normal{0};
    float clippingDistance;
};

/** Generated when two objects collide. Stores which two objects collided.
 */
struct ObjectHit
{
    ObjectHit(unsigned i, unsigned j ) : id1(i), id2(j) {}
    unsigned id1;
    unsigned id2;
};

class MeshComponent;

/** Detects and handles collisions for all entities with a collision component.
 */
class CollisionSystem
{
public:
    CollisionSystem();
    /** Checks all CollisionComponents to see if the ray goes through any of their collision spheres.
        returns the first one it finds, which will be the one with the lowest index, and not neccesarily the closest one.
       @param traceOrigin: Origin of the ray in world space
       @param traceEnd: End of the ray in world space.
       @return Index of the entity hit by the ray.
     */
    int checkRaytraceCollison(gsl::Vector3D traceOrigin, gsl::Vector3D traceEnd);

    /** Checks all entities with a CollisionComponent against each other to see if anyone of them are colliding.
        For each collision found, a ObjectHit is generated, and sent directly to the ScriptSystem for handling.
     */
    void doCollisionDetection();

    /** Checks if a ray is colliding with a Mesh, and returns the position on that mesh where the collision hits.
        This algorithm uses the mesh in local space, and will therefore only be correct in world space if the mesh is situated
        in origo.
        Uses an algorithm called Möller–Trumbore intersection algorithm.
       @param traceOrigin: Origin of the trace
       @param traceVec: Direction of the trace
       @param meshComp: The MeshComponent we are checking against.
       @return The collision point between the ray and the mesh
     */
    gsl::Vector3D rayMeshCollision(gsl::Vector3D traceOrigin, gsl::Vector3D traceVec, MeshComponent* meshComp);

    /** Uses barysentric coordinates to check all CollisionComponents against all landscapes.
        If a collision is found, a LandscapeHit is generated and stored in mLandscapeCollisions.
     */
    void doCollisionDetectionWithLandscape();

    /** Handles the collisions stored in mLandscapeCollisions.
        Sends them to the appropriate function in  PhysicsSystem.
     */
    void handleLandscapeCollisions();

    ///Stores the LandscapeCollisions found this frame.
    std::vector<LandscapeHit> mLandscapeCollisions;
};

#endif // COLLISIONSYSTEM_H
