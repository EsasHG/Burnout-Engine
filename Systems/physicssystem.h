#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

struct LandscapeHit;
class PhysicsComponent;
/** Handles physics for all entities with a PhysicsComponent
 */
class PhysicsSystem
{
public:
    PhysicsSystem();

    /** Handles a collision with a landscape.
       This is designed to make the entity roll along a landscape, affected by gravity.
       Does not currently work while an entity is being affected by input.
       @param hit: The hit information for the collision.
     */
    void physicsLandscapeHit(LandscapeHit hit);

    /** Applies gravity to an entity's acceleration in its PhysicsComponent.

       @param inID: The entity to apply gravity to.
     */
    void freeFall(unsigned inID);

    /** Handles input in a way so that the entity follows the landscape with barycentric coordinates.
       @param inID: the entity id.
       @param inputVector: The direction the entity is to move. Expected to be in x-z plane.
     */
    void handleInput(unsigned inID, gsl::Vector3D inputVector);

};

#endif // PHYSICSSYSTEM_H
