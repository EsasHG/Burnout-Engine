#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "basecomponent.h"
#include "Sound/soundsource.h"
/** Component needed for an entity to be controllable.
 */
class InputComponent : public BaseComponent
{
public:
    InputComponent(unsigned inID) : BaseComponent(inID){}
    ~InputComponent()override {}

    gsl::Vector3D inputVector{0.f,0.f,0.f};
    virtual QJsonObject toJson() const override;
    virtual void fromJson(QJsonObject obj) override;

};
#endif // INPUTCOMPONENT_H
