#ifndef TRANSFORMSYSTEM_H
#define TRANSFORMSYSTEM_H

#include "innpch.h"

namespace gsl {

class Vector3D;
}
class TransformComponent;
/** Calculates and alters entity transforms.
    The only class that is allowed to access the variables in TransformComponent class.
    All getting and setting for TransformComponent values has to go through here.
 */
class TransformSystem
{
public:
    TransformSystem();

    /** Checks to see if the transform component is valid, then sets the position.
        Also updates the sound source if there is one.
     * @param tComp: the component to use.
     * @param inPos: the position to set to.
     */
    void setPosition(TransformComponent* tComp,gsl::Vector3D inPos);

    /** Checks to see if the id has a transform component, then sets the position.
        Also updates the sound source if there is one.
     * @param inID: the id to use.
     * @param inPos: the position to set to.
     */
    void setPosition(unsigned inID, gsl::Vector3D inPos);

    /** Checks to see if the transform component is valid, then sets the rotation.
        Also updates the sound source if there is one.
     * @param tComp: the component to use.
     * @param inRot: the rotation to set to.
     */
    void setRotation(TransformComponent* tComp, gsl::Vector3D inScale);

    /** Checks to see if the id has a transform component, then sets the rotation.
     * @param inID: the id to use.
     * @param inRot: the rotation to set to.
     */
    void setRotation(unsigned inID, gsl::Vector3D inRot);

    /** Checks to see if the transform component is valid, then sets the scale.
     * @param tComp: the component to use.
     * @param inScale: the scale to set to.
     */
    void setScale(TransformComponent* tComp, gsl::Vector3D inScale);

    /** Checks to see if the id has a transform component, then sets the scale.
     * @param inID: the id to use.
     * @param inScale: the scale to set to.
     */
    void setScale(unsigned inID, gsl::Vector3D inScale);

    /** Checks to see if the id has a transform component, then gets the entity's position in world space.
     * @param inID: the entity ID.
     * @return if a transform component is found, return world position. Otherwise return gsl::vector(0).
     */
    gsl::Vector3D getWorldPosition(unsigned inID);

    /** Checks to see if the id has a transform component, then gets the entity's position in world space.
        Currently untested.
     * @param inID: the entity ID.
     * @return if a transform component is found, return world position.
     */
    gsl::Vector3D getWorldRotation(unsigned inID);

    /** Checks to see if the id has a transform component, then gets the entity's position in world space.
     * @param inID: the entity ID.
     * @return if a transform component is found, return world position. Otherwise return gsl::vector(1,1,1).
     */
    gsl::Vector3D getWorldScale(unsigned inID);

    /** Checks for transform component, then finds the local position for the ID.
        If the id is a child of another entity, you will get the position relative to that entity.
     * @param inID: the entity ID.
     * @return if a transform component is found, return world position. Otherwise return gsl::vector(0,0,0).
     */
    gsl::Vector3D getLocalPosition(unsigned inID);

    /** Checks for transform component, then finds the local rotation for the ID.
        If the id is a child of another entity, you will get the rotation relative to that entity.
     * @param inID: the entity ID.
     * @return if a transform component is found, return world rotation. Otherwise return gsl::vector(0,0,0).
     */
    gsl::Vector3D getLocalRotation(unsigned inID);

    /** Checks for transform component, then finds the local scale for the ID.
        If the id is a child of another entity, you will get the scale relative to that entity.
     * @param inID: the entity ID.
     * @return if a transform component is found, return world scale. Otherwise return gsl::vector(1,1,1).
     */
    gsl::Vector3D getLocalScale(unsigned inID);

    /** Checks the component, then translates the position.
        Also updates soundsource if there is one.
     * @param tComp: the component to translate.
     * @param inPos: the vector to translate with.
     */
    void addPosition(unsigned inID, gsl::Vector3D inPos);

    /** Checks for transformcomponent, then translates the position.
        Also updates soundsource if there is one.
     * @param inID: the id to translate
     * @param inPos: the vector to translate with
     */
    void addPosition(TransformComponent* tComp,gsl::Vector3D inPos);

    /** Checks the component, then adds to rotation.
     * @param tComp: the component to add to.
     * @param inRot: the rotation to add.
     */
    void addRotation(unsigned inID, gsl::Vector3D inRot);

    /** Checks for component, then adds to rotation.
     * @param inID: the id to add to.
     * @param inRot: the rotation to add.
     */
    void addRotation(TransformComponent* tComp, gsl::Vector3D inRot);

    /** Checks the component, then adds to scale.
     * @param tComp: the component to add to.
     * @param inScale: the scale to add.
     */
    void addScale(TransformComponent* tComp, gsl::Vector3D inScale);

    /** Checks for component, then adds to scale
     * @param inID: the id to add to.
     * @param inScale: the scale to add.
     */
    void addScale(unsigned inID, gsl::Vector3D inScale);

    /** Finds the transformation/model matrix for the component.
        Used in RenderSystem to calculate the model matrix.
     * @param transformComp: the component to find the matrix of.
     * @return the full transform/model matrix for the component.
     */
    gsl::Matrix4x4 getTransformationMatrix(TransformComponent *transformComp);

    gsl::Matrix4x4 getRotationMatrix(unsigned inID);
private:

    /** Gets the local transform matrix for the component.
       Only meant to be used to find the world transform matrix in getTransformationMatrix.
     * @param transformComp: the component to find the matrix of.
     * @return the local transformation matrix for the component.
     */
    gsl::Matrix4x4 getObjectTransformationMatrix(TransformComponent *transformComp);
};

#endif // TRANSFORMSYSTEM_H
