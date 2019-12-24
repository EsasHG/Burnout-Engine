#ifndef QUATERNION_H
#define QUATERNION_H

namespace gsl
{

class Matrix4x4;
/** A Quaternion class for storing the 4 values in a quaternion, with some useful functionality for altering them
 */
class Quaternion
{
public:
    Quaternion();
    Quaternion(float w, float x, float y, float z);
    Quaternion(float w, gsl::Vector3D xyz);

    Quaternion operator -(const Quaternion &rhs) const;
    Quaternion operator +(const Quaternion &rhs) const;
    Quaternion operator +=(const Quaternion &rhs);

    float& operator[](const int index)
    {
        assert(index <4 && index >=0);

        switch (index) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
        }
        return x;   //to silence compiler warnings
    }

    //OEF: Adding these again to be able to debug easier. cout does not print until program is finished!
    friend QDebug operator<<(QDebug out, const Quaternion &rhs)
    {
        out << "(" << rhs.w << "," << rhs.x << "," << rhs.y << "," << rhs.z <<")";
        return  out;
    }        //    QVector3D getQVector() const;   //for use with qDebug()

    /** Converts the quaternion into a rotation matrix.
       @return A matrix does the rotation the quaternion describes.
     */
    Matrix4x4 toMatrix();
    /** Finds the conjugate of the quaternion.
       @return The quaternion's conjugate, or its reverse if you will.
     */
    Quaternion getConj();

    /** Calculates the norm, or the length of the quaternion.
       @return The quaternion's norm.
     */
    float getNorm();
    /** Returns the normalized version of the quaternion.
       @return The quaternion, but normalized.
     */
    Quaternion getNormalQuat();
    /** Normalizes the quaternion.
     */
    void normalize();

    float w;
    float x;
    float y;
    float z;

    Quaternion &operator /=(const float f);
    Quaternion operator *(const Quaternion &rhs) const;
    /** Converts the quaternion to yaw, pitch and roll in radians.
       @return A vector with Yaw, Pitch and Roll
     */
    gsl::Vector3D toEulerRotation();
    /** Converts the quaternion to yaw, pitch and roll in radians.
       @return A vector with Yaw, Pitch and Roll
     */
    gsl::Vector3D toEulerRotationDeg();
    /** Makes a quaternion from a vector with yaw, pitch and roll values.
       @param inRot: The rotation to convert from, expected in radians.
     */
    void fromEulerRotation(gsl::Vector3D inRot);
};
}//namespace

#endif // QUATERNION_H
