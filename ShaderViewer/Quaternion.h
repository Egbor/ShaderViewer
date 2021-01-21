#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "Vector.h"

class Quaternion {
public:
	Quaternion(XMVECTOR quaternion);
	Quaternion(float x, float y, float z, float w);
	Quaternion(float eulerX, float eulerY, float eulerZ);
	Quaternion(Vector3 eulerAngles);
	Quaternion(Vector3 axis, float angle);

	static Quaternion AxisAngle(Vector3 axis, float angle);
	static Quaternion Euler(float x, float y, float z);
	static Quaternion Euler(Vector3 euler);
	static Quaternion LookRotate(Vector3 &to, Vector3 &up = Vector3::up);
	static Vector3 RotatePoint(Quaternion rot, Vector3 point);

	Vector3 ToEulerAngles() const;
	Vector3 ToVector() const;

	Vector3 GetLocalForword() const;
	Vector3 GetLocalUp() const;
	Vector3 GetLocalRight() const;

	Quaternion Inverse();

	XMVECTOR GetSIMD() const;

	friend Quaternion operator*(Quaternion q1, Quaternion q2);

private:
	static Vector3 ToEulerAngles(float x, float y, float z, float w);
	void Set(float x, float y, float z, float w);

	Vector3 mEulerAngles;
	XMVECTOR mSMIDQuaternion;
};

#endif // !_QUATERION_H_
