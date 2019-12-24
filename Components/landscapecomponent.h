#ifndef LANDSCAPECOMPONENT_H
#define LANDSCAPECOMPONENT_H

#include "basecomponent.h"

/** Component given to landscape entities.
   Holds the landscapes width and height, and the highest and lowest value in each axis
 */
class LandscapeComponent : public BaseComponent
{
public:
    LandscapeComponent(unsigned inID) : BaseComponent(inID){}
    ~LandscapeComponent() override{}
    virtual QJsonObject toJson() const override;
    virtual void fromJson(QJsonObject obj) override;

    float xmin=-60.0f, xmax=60.0f, zmin=-60.0f, zmax=60.0f, h=2.5f;
    float ymin= 0, ymax = 0;
    unsigned mWidth;
    unsigned mHeight;

    QString filename = "";
};


#endif // LANDSCAPECOMPONENT_H
