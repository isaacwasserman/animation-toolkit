#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"

namespace atkmath {

Matrix3 xRotationMatrix(const float& theta){
   return Matrix3(
      1, 0,          0,
      0, cos(theta), -1 * sin(theta),
      0, sin(theta), cos(theta)
   );
}

Matrix3 yRotationMatrix(const float& theta){
   return Matrix3(
      cos(theta),      0, sin(theta),
      0,               1, 0,
      -1 * sin(theta), 0, cos(theta)
   );
}

Matrix3 zRotationMatrix(const float& theta){
   return Matrix3(
      cos(theta), -1 * sin(theta), 0,
      sin(theta), cos(theta),      0,
      0,          0,               1
   );
}

Vector3 Matrix3::toEulerAnglesXYZ() const
{
   float alpha,theta,beta;
   theta = asin(m13);
   if(abs(m13) != 1){
      beta = atan2(-1 * m12, m11);
      alpha = atan2(-1 * m23,m33);
   }
   else {
      beta = 0;
      alpha = atan2(m21,m22);
   }
   return Vector3(alpha,theta,beta);
}

Vector3 Matrix3::toEulerAnglesXZY() const
{
   float alpha,theta,beta;
   beta = asin(-m12);
   if(abs(m12) != 1){
      theta = atan2(m13, m11);
      alpha = atan2(m32,m22);
   }
   else {
      theta = 0;
      alpha = atan2(m31,m21);
   }
   return Vector3(alpha,theta,beta);
}

Vector3 Matrix3::toEulerAnglesYXZ() const
{
   float alpha,theta,beta;
   alpha = asin(-m23);
   if(abs(m23) != 1){
      theta = atan2(m13, m33);
      beta = atan2(m21, m22);
   }
   else {
      theta = 0;
      beta = atan2(m31,m11);
   }
   return Vector3(alpha,theta,beta);
}

Vector3 Matrix3::toEulerAnglesYZX() const
{
   float alpha,theta,beta;
   beta = asin(m21);
   if(abs(m21) != 1){
      theta = atan2(-1 * m31, m11);
      alpha = atan2(-1 * m23, m22);
   }
   else {
      theta = 0;
      alpha = atan2(m32,m33);
   }
   return Vector3(alpha,theta,beta);
}

Vector3 Matrix3::toEulerAnglesZXY() const
{
   float alpha,theta,beta;
   alpha = asin(m32);
   if(abs(m32) != 1){
      theta = atan2(-1 * m31, m33);
      beta = atan2(-1 * m12, m21);
   }
   else {
      theta = 0;
      beta = atan2(m21,m11);
   }
   return Vector3(alpha,theta,beta);
}

Vector3 Matrix3::toEulerAnglesZYX() const
{ 
   float alpha,theta,beta;
   theta = asin(-1 * m31);
   if(abs(m31) != 1){
      beta = atan2(m21,m11);
      alpha = atan2(m32,m33);
   }
   else {
      beta = 0;
      alpha = atan2(m12,m13);
   }
   return Vector3(alpha,theta,beta);
}

void Matrix3::fromEulerAnglesXYZ(const Vector3& angleRad)
{
   *this = xRotationMatrix(angleRad[0]) * yRotationMatrix(angleRad[1]) * zRotationMatrix(angleRad[2]);
}

void Matrix3::fromEulerAnglesXZY(const Vector3& angleRad)
{
   *this = xRotationMatrix(angleRad[0]) * zRotationMatrix(angleRad[2]) * yRotationMatrix(angleRad[1]);
}

void Matrix3::fromEulerAnglesYXZ(const Vector3& angleRad)
{
   *this =  yRotationMatrix(angleRad[1]) * xRotationMatrix(angleRad[0]) * zRotationMatrix(angleRad[2]);
}

void Matrix3::fromEulerAnglesYZX(const Vector3& angleRad)
{
   *this =  yRotationMatrix(angleRad[1]) * zRotationMatrix(angleRad[2]) * xRotationMatrix(angleRad[0]);
}

void Matrix3::fromEulerAnglesZXY(const Vector3& angleRad)
{
   *this =  zRotationMatrix(angleRad[2]) * xRotationMatrix(angleRad[0]) * yRotationMatrix(angleRad[1]);
}

void Matrix3::fromEulerAnglesZYX(const Vector3& angleRad)
{
   *this =  zRotationMatrix(angleRad[2]) * yRotationMatrix(angleRad[1]) * xRotationMatrix(angleRad[0]);
}

void Matrix3::toAxisAngle(Vector3& axis, double& angleRad) const
{
   // TODO
}

void Matrix3::fromAxisAngle(const Vector3& axis, double angleRad)
{
   // TODO
   *this = Identity;
}

}