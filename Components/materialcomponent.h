#ifndef MATERIAL_H
#define MATERIAL_H

#include "Components/basecomponent.h"
#include "vector3d.h"

class Texture;
/** Needed to render a mesh.
   Holds the shader to be used, as well as the optional color and texture values.
 */
class MaterialComponent : public BaseComponent
{
public:
    MaterialComponent(unsigned inID) : BaseComponent(inID) {}
    ~MaterialComponent() override{}
    virtual QJsonObject toJson() const override;
    virtual void fromJson(QJsonObject obj) override;

    gsl::Vector3D mAmbientColor{0.3f, 0.3f, 0.3f};
    gsl::Vector3D mDiffuseColor{1.f, 1.f, 1.f};
    gsl::Vector3D mSpecularColor{0.5f, 0.5f, 0.5f};
    float mShininess = 32;
    std::string mTextureFile;
    Texture* mTexture = nullptr;

    Shader *mShader{nullptr};
};

#endif // MATERIAL_H
