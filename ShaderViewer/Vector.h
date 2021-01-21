#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "MathCommon.h"

class Vector3 {
	friend class Transform;
	friend class Matrix4x4;
public:
	Vector3(float x, float y ,float z);
	Vector3(const Vector3& vector);

	static Vector3 back;
	static Vector3 down;
	static Vector3 forword;
	static Vector3 left;
	static Vector3 right;
	static Vector3 up;
	static Vector3 one;

	static bool Equal(Vector3 &v1, Vector3 &v2);
	static float Angle(Vector3 v1, Vector3 v2);
	static float Dot(Vector3 v1, Vector3 v2);
	static Vector3 Cross(Vector3 v1, Vector3 v2);

	friend Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
	friend Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
	friend Vector3 operator*(const Vector3 &v, float value);
	friend Vector3 operator/(const Vector3 &v, float value);

	float Magnitude() const;
	Vector3 Normalize();
	void Set(float x, float y, float z);
	void Set(const Vector3 &vector);

	float GetX() const;
	float GetY() const;
	float GetZ() const;

	XMVECTOR GetSIMD()const;

private:
	XMVECTOR mSIMDVector;

	float mMagnitude;
};

#endif // !_VECTOR_H_
