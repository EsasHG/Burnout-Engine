#include "quaternion.h"
#include <cmath>
namespace gsl
{
Quaternion::Quaternion()
{

}
Quaternion::Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z)
{

}

Quaternion::Quaternion(float w, Vector3D xyz) : w(w), x(xyz.x), y(xyz.y), z(xyz.z)
{

}

Quaternion Quaternion::operator*(const Quaternion& rhs) const
{
    return Quaternion(
                w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z,
                x*rhs.w + w*rhs.x + y*rhs.z - z*rhs.y,
                w*rhs.y - x*rhs.z + y*rhs.w + z*rhs.x,
                w*rhs.z + x*rhs.y - y*rhs.x + z*rhs.w
    );
}

Quaternion& Quaternion::operator/=(const float f)
{
    w/=f;
    x/=f;
    y/=f;
    z/=f;
    return *this;
}

Quaternion Quaternion::operator+(const Quaternion& rhs) const
{
    return {w + rhs.w,x + rhs.x, y + rhs.y, z + rhs.z};
}

Quaternion Quaternion::operator +=(const Quaternion &rhs)
{
    w+= rhs.w;
    x+= rhs.x;
    y+= rhs.y;
    z+= rhs.z;
    return *this;
}


Quaternion Quaternion::operator-(const Quaternion& rhs) const
{
    return {w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z};
}


Matrix4x4 Quaternion::toMatrix()
{
    //assumes a unit quaternion
    normalize();
    Matrix4x4 temp = Matrix4x4({
                  1-2*y*y - 2*z*z, 2*x*y - 2*z*w,   2*x*z + 2*y*w,      0,
                  2*x*y + 2*z*w  , 1-2*x*x - 2*z*z, 2*y*z - 2*x*w,      0,
                  2*x*z - 2*y*w  , 2*y*z + 2*x*w,   1-2*x*x - 2*y*y,    0,
                  0,               0,               0,                  1
              });
    return temp;
}

Quaternion Quaternion::getConj()
{
    return Quaternion(w,-x,-y,-z);
}

float Quaternion::getNorm()
{
    return std::sqrt(w*w + x*x + y*y + z*z);
}

Quaternion Quaternion::getNormalQuat()
{
    float length = getNorm();
    Quaternion temp = *this;
    temp.w/=length;
    temp.x/=length;
    temp.y/=length;
    temp.z/=length;
    return temp;

}

void Quaternion::normalize()
{
    float length = getNorm();
    w/=length;
    x/=length;
    y/=length;
    z/=length;

}
void Quaternion::fromEulerRotation(gsl::Vector3D inRot)
{
    float cy = static_cast<float>(std::cos(inRot.z*0.5f));
    float sy = static_cast<float>(std::sin(inRot.z*0.5f));
    float cp = static_cast<float>(std::cos(inRot.y*0.5f));
    float sp = static_cast<float>(std::sin(inRot.y*0.5f));
    float cr = static_cast<float>(std::cos(inRot.x*0.5f));
    float sr = static_cast<float>(std::sin(inRot.x*0.5f));

    w = cy*cp*cr + sy*sp*sr;
    x = cy*cp*sr - sy*sp*cr;
    y = sy*cp*sr + cy*sp*cr;
    z = sy*cp*cr - cy*sp*sr;

    normalize();

    // Abbreviations for the various angular functions
}

gsl::Vector3D Quaternion::toEulerRotation()
{
    // pitch (y-axis rotation)
    float ytemp;
    double sinp = 2 * (w * y - z * x);
    if (std::abs(sinp) >= 1)
        ytemp = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        ytemp = std::asin(sinp);


    return gsl::Vector3D(
                std::atan2(2*(w*x + y*z) , 1 - 2*(x*x + y*y)),
                ytemp,
                std::atan2(2*(w*z + x*y), 1-2*(y*y + z*z))
            );
}

gsl::Vector3D Quaternion::toEulerRotationDeg()
{
    // pitch (y-axis rotation)
    float ytemp;
    double sinp = 2 * (w * y - z * x);
    if (std::abs(sinp) >= 1)
        ytemp = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        ytemp = std::asin(sinp);


    gsl::Vector3D ret = gsl::Vector3D(
            std::atan2(2*(w*x + y*z) , 1 - 2*(x*x + y*y)),
            ytemp,
            std::atan2(2*(w*z + x*y), 1-2*(y*y + z*z))
        );
    ret.x = gsl::rad2deg(ret.x);
    ret.y = gsl::rad2deg(ret.y);
    ret.z = gsl::rad2deg(ret.z);
    return ret;
}

}

