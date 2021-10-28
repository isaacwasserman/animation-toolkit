#include <algorithm>
#include "atkmath/quaternion.h"
#include "atkmath/matrix3.h"
#include "atkmath/vector3.h"

namespace atkmath {

Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, double t)
{
	float dp = q0.Dot(q0,q1);
	float omega = acos(dp);
	Quaternion n1 = (sin((1 - t)*omega)/sin(omega))*q0;
	Quaternion n2 = (sin(t*omega)/sin(omega))*q1;
	return n1 + n2;
}

void Quaternion::toAxisAngle (Vector3& axis, double& angleRad) const
{
	angleRad = acos(this->mW) * 2;
	axis = Vector3(this->mX,this->mY,this->mZ) / sin(angleRad / 2.0f);
}

void Quaternion::fromAxisAngle (const Vector3& axis, double angleRad)
{
	Vector3 xyz = axis * sin(angleRad / 2.0f);
	float w = cos(angleRad / 2.0f);
	*this = Quaternion(xyz[0],xyz[1],xyz[2],w);
	normalize();
}

Matrix3 Quaternion::toMatrix () const
{
	float x = this->mX;
	float y = this->mY;
	float z = this->mZ;
	float w = this->mW;

	Matrix3 m(
		1-2*(pow(y,2)+pow(z,2)), 2*(x*y-w*z),             2*(x*z+w*y),
		2*(x*y+w*z),             1-2*(pow(x,2)+pow(z,2)), 2*(y*z-w*x),
		2*(x*z-w*y),             2*(y*z+w*x),             1-2*(pow(y,2)+pow(x,2))
	);

	return m;
}

void Quaternion::fromMatrix(const Matrix3& rot)
{
	float x;
	float y;
	float z;
	float w;

	float w2 = 0.25f * (rot.m11 + rot.m22 + rot.m33 + 1);
	float x2 = 0.25f * (1 + rot.m11 - rot.m22 - rot.m33);
	float y2 = 0.25f * (1 - rot.m11 + rot.m22 - rot.m33);
	float z2 = 0.25f * (1 - rot.m11 - rot.m22 + rot.m33);
	
	std::vector<float> squares = {w2,x2,y2,z2};

	float max = *std::max_element(squares.begin(),squares.end());

	if(max == w2){
		w = sqrt(w2);
		x = ((rot.m32 - rot.m23) / 4.0f) / w;
		y = ((rot.m13 - rot.m31) / 4.0f) / w;
		z = ((rot.m21 - rot.m12) / 4.0f) / w;
		// std::cout << "w2 is max" << std ::endl;
	} else if(max == x2){
		x = sqrt(x2);
		// ERROR WAS HERE
		y = ((rot.m21 + rot.m12) / 4.0f) / x;
		z = ((rot.m13 + rot.m31) / 4.0f) / x;
		w = ((rot.m32 - rot.m23) / 4.0f) / x;
		// std::cout << "x2 is max" << std ::endl;
	} else if(max == y2){
		y = sqrt(y2);
		x = ((rot.m12 + rot.m21) / 4.0f) / y;
		z = ((rot.m23 + rot.m32) / 4.0f) / y;
		w = ((rot.m13 - rot.m31) / 4.0f) / y;
		// std::cout << "y2 is max" << std ::endl;
	}
	 else if(max == z2){
		z = sqrt(z2);
		x = ((rot.m13 + rot.m31) / 4.0f) / z;
		y = ((rot.m23 + rot.m32) / 4.0f) / z;
		w = ((rot.m21 - rot.m12) / 4.0f) / z;
		// std::cout << "z2 is max" << std ::endl;
	}

	*this = Quaternion(x,y,z,w);
	normalize();
}

}