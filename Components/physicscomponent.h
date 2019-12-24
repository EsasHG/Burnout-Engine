#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "basecomponent.h"
#include "GSL/matrix4x4.h"
#include "GSL/vector3d.h"
#include "Sound/soundsource.h"

/** Holds velocity, acceleration, and other variables needed for an entity to be affected by physics.
 */
class PhysicsComponent : public BaseComponent
{
public:
    PhysicsComponent(unsigned inID) : BaseComponent(inID){}
    ~PhysicsComponent() override{}
    float speed{0.1f};
private:
    gsl::Vector3D velocity{0};
    gsl::Vector3D acceleration{0};
    gsl::Vector3D inputAcceleration{0};
    gsl::Vector3D surfaceNormal{0};
    friend class PhysicsSystem;
    friend class PhysicsWidget;

    virtual QJsonObject toJson() const override;
    virtual void fromJson(QJsonObject obj) override;
};

#endif // PHYSICSCOMPONENT_H
