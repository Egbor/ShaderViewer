#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Vector.h"
#include "Quaternion.h"

class Transform {
	using Position = XMFLOAT3;
	using Vector = XMVECTOR;
public:
	Transform();

	void Translate(float x, float y, float z);
	void Translate(Vector3 direction);

	void Rotate(float x, float y, float z);
	void Rotate(Vector3 eulerAngles);
	void Rotate(Vector3 axis, float angle);

	void Scale(float x, float y, float z);
	void Scale(Vector3 scale);

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetRotation(Vector3 axis, float angle);
	void SetScaling(float x, float y, float z);

	void LookAt(Transform target, Vector3 up = Vector3::up);
	void RotateAround(Vector3 point, Vector3 axis, float angle);

	Vector3 GetPosition() const;
	Quaternion GetRotation() const;
	Vector3 GetScaling() const;

	Vector3 GetForword() const;
	Vector3 GetRight() const;
	Vector3 GetUp() const;

	XMMATRIX GetWorldMatrix() const;

private:
	void LocalToWorld();

	Vector3 mEulerAngles;
	Vector3 mForworld;
	Vector3 mRight;
	Vector3 mUp;

	Vector3 mPosition;
	Vector3 mScaling;
	Quaternion mRotation;
};

#endif // !_TRANSFORM_H_
