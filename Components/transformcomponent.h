#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "basecomponent.h"
#include "GSL/matrix4x4.h"
#include "GSL/vector3d.h"
#include "GSL/quaternion.h"

/** Holds the transform variables of an entity: Position, Rotation and Scale.
   Also keeps track of the entity's parent-child relationships.
 */
class TransformComponent : public BaseComponent
{
public:
    TransformComponent(unsigned inID) : BaseComponent(inID){ }//mMatrix.setToIdentity();}
    ~TransformComponent() override{}
    std::vector<unsigned> mChildren;
    int mParent = -1;

    virtual void fromJson(QJsonObject obj) override;
    virtual QJsonObject toJson() const override;
private:

    gsl::Vector3D position{0,0,0};
    gsl::Vector3D scale{1,1,1};
    gsl::Quaternion rotation{1,0,0,0};
    friend class TransformSystem;
};
#endif // TRANSFORMCOMPONENT_H
