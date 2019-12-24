#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include "basecomponent.h"
#include "GSL/matrix4x4.h"
#include "GSL/vector3d.h"
#include "Sound/soundsource.h"
/** Component needed for an entity to have collision. Currently only supports sphere collision.
 */
class CollisionComponent : public BaseComponent
{
public:
    CollisionComponent(unsigned inID) : BaseComponent(inID){}
    ~CollisionComponent() override{}
    virtual QJsonObject toJson() const override;
    virtual void fromJson(QJsonObject obj) override;

    gsl::Vector3D localOrigin{0.f,0.f,0.f};
    float radius = 1.f;
    friend class CollisionSystem;


};

#endif // COLLISIONCOMPONENT_H
