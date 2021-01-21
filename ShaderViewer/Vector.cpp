#include "Vector.h"

Vector3 Vector3::back(0.0f, 0.0f, -1.0f);
Vector3 Vector3::down(0.0f, -1.0f, 0.0f);
Vector3 Vector3::forword(0.0f, 0.0f, 1.0f);
Vector3 Vector3::left(-1.0f, 0.0f, 0.0f);
Vector3 Vector3::right(1.0f, 0.0f, 0.0f);
Vector3 Vector3::up(0.0f, 1.0f, 0.0f);
Vector3 Vector3::one(1.0f, 1.0f, 1.0f);

Vector3::Vector3(float x, float y, float z) {
	Set(x, y, z);
}

Vector3::Vector3(const Vector3& vector) {
	mSIMDVector = vector.mSIMDVector;
	mMagnitude = vector.mMagnitude;
}

XMVECTOR Vector3::GetSIMD() const {
	return mSIMDVector;
}

float Vector3::GetX() const {
	return XMVectorGetX(mSIMDVector);
}

float Vector3::GetY() const {
	return XMVectorGetY(mSIMDVector);
}

float Vector3::GetZ() const {
	return XMVectorGetZ(mSIMDVector);
}

Vector3 Vector3::Normalize() {
	mSIMDVector = XMVector3Normalize(mSIMDVector);
	mMagnitude = 1.0f;
	return *this;
}

void Vector3::Set(float x, float y, float z) {
	mSIMDVector = XMVectorSet(x, y, z, 0.0f);
	mMagnitude = sqrtf(x * x + y * y + z * z);
}

void Vector3::Set(const Vector3 &vector) {
	mSIMDVector = vector.mSIMDVector;
	mMagnitude = vector.mMagnitude;
}

float Vector3::Magnitude() const {
	return mMagnitude;
}

bool Vector3::Equal(Vector3& v1, Vector3& v2) {
	XMVECTOR result = XMVectorEqual(v1.mSIMDVector, v2.mSIMDVector);
	return XMVectorGetX(result) && XMVectorGetY(result) && XMVectorGetZ(result);
}

float Vector3::Angle(Vector3 v1, Vector3 v2) {
	XMVECTOR angle = XMVector3AngleBetweenVectors(v1.mSIMDVector, v2.mSIMDVector);
	return XMVectorGetX(angle);
}

float Vector3::Dot(Vector3 v1, Vector3 v2) {
	XMVECTOR result = XMVector3Dot(v1.mSIMDVector, v2.mSIMDVector);
	return XMVectorGetX(result);
}

Vector3 Vector3::Cross(Vector3 v1, Vector3 v2) {
	XMVECTOR result = XMVector3Cross(v1.mSIMDVector, v2.mSIMDVector);
	return Vector3(XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result));
}

Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	XMVECTOR result = XMVectorAdd(v1.mSIMDVector, v2.mSIMDVector);
	return Vector3(XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result));
}

Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	XMVECTOR result = XMVectorSubtract(v1.mSIMDVector, v2.mSIMDVector);
	return Vector3(XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result));
}

Vector3 operator*(const Vector3& v, float value) {
	XMVECTOR result = XMVectorScale(v.mSIMDVector, value);
	return Vector3(XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result));
}

Vector3 operator/(const Vector3& v, float value) {
	XMVECTOR result = XMVectorScale(v.mSIMDVector, 1.0f / value);
	return Vector3(XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result));
}