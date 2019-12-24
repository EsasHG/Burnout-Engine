#ifndef CAMERA_H
#define CAMERA_H
#include "Components/basecomponent.h"
#include <QDebug>
/** Component that holds all the camera variables.
 */
class CameraComponent : public BaseComponent
{
public:
    CameraComponent(unsigned inID) :  BaseComponent(inID)
    { }
    ~CameraComponent() override {}
    virtual QJsonObject toJson() const override;
    virtual void fromJson(QJsonObject obj) override;

    gsl::Matrix4x4 mViewMatrix;
    gsl::Matrix4x4 mProjectionMatrix;

    gsl::Vector3D mForward{0.f, 0.f, -1.f};
    gsl::Vector3D mRight{1.f, 0.f, 0.f};
    gsl::Vector3D mUp{0.f, 1.f, 0.f};

    float mPitch{0.f};
    float mYaw{0.f};
    float mSpeed{0.f};
    float sensitivity{0.1f};

    //third person camera stuff
    int entityToLookAt = -1;
    bool bIsThirdPersonCamera = false;
    bool bShowFrustum = false;


};

#endif // CAMERA_H
