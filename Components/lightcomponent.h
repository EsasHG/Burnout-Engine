#ifndef LIGHT_H
#define LIGHT_H
#include "Components/basecomponent.h"

/** Component that defines a light that can be used with the PhongTextureShader.
 */
class LightComponent : public BaseComponent
{
public:
    LightComponent(unsigned inID) :BaseComponent(inID) {}
    ~LightComponent() override{}

    virtual QJsonObject toJson() const override;
    virtual void fromJson(QJsonObject obj) override;

    gsl::Vector3D mAmbientColor{0.5f, 0.5f, 0.5f};
    gsl::Vector3D mDiffuseColor{0.8f, 0.8f, 0.8f};
    gsl::Vector3D mSpecularColor{1.f, 1.f, 1.f};

//    GLfloat mLightStrength{0.8f};
//    GLfloat mAmbientStrength{0.3f};
//    GLfloat mSpecularStrenght{0.3f};
//    GLint mSpecularExponent{4};
};

#endif // LIGHT_H
