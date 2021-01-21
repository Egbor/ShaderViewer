#include "Transform.h"
#include "Matrix4x4.h"

Transform::Transform() 
	: mPosition(0.0f, 0.0f, 0.0f), mRotation(0.0f, 0.0f, 0.0f, 1.0f), mScaling(1.0f, 1.0f, 1.0f), mEulerAngles(0.0f, 0.0f, 0.0f),
	mForworld(0.0f, 0.0f, 1.0f), mRight(1.0f, 0.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f) {

}

void Transform::Translate(float x, float y, float z) {
	Vector3 translate(x, y, z);
	mPosition = mPosition + translate;
}

void Transform::Translate(Vector3 direction) {
	mPosition = mPosition + direction;
}

void Transform::Rotate(float x, float y, float z) {
	Vector3 rotate(x, y, z);
	mEulerAngles = mEulerAngles + rotate;
	//Quaternion q = Quaternion::Euler(x, y, z);
	//q * Quaternion(mForworld.GetSIMD()) * q.Inverse();
	//q * Quaternion(mUp.GetSIMD()) * q.Inverse();
	mRotation = Quaternion::Euler(mEulerAngles);
}

void Transform::Rotate(Vector3 eulerAngles) {
	mEulerAngles = mEulerAngles + eulerAngles;
	mRotation = Quaternion::Euler(mEulerAngles);
}

void Transform::Rotate(Vector3 axis, float angle) {
	Quaternion axisAngle = Quaternion::AxisAngle(axis, angle);
	mEulerAngles = mEulerAngles + axisAngle.ToEulerAngles();
	mRotation = mRotation * axisAngle;
}

void Transform::Scale(float x, float y, float z) {
	Vector3 scale(x, y, z);
	mScaling = mScaling + scale;
}

void Transform::Scale(Vector3 scale) {
	mScaling = mScaling + scale;
}

void Transform::SetPosition(float x, float y, float z) {
	mPosition.Set(x, y, z);
}

void Transform::SetRotation(float x, float y, float z) {
	mEulerAngles.Set(x, y, z);
	mRotation = Quaternion::Euler(x, y, z);
}

void Transform::SetRotation(Vector3 axis, float angle) {
	mRotation = Quaternion::AxisAngle(axis, angle);
	mEulerAngles = mRotation.ToEulerAngles();
}

void Transform::SetScaling(float x, float y, float z) {
	mScaling.Set(x, y, z);
}

void Transform::LookAt(Transform target, Vector3 up) {
	Vector3 axisZ = (target.mPosition - mPosition).Normalize();
	mRotation = Quaternion::LookRotate(axisZ, up);
	mEulerAngles = mRotation.ToEulerAngles();
}

void Transform::RotateAround(Vector3 point, Vector3 axis, float angle) {
	Matrix4x4 rotateMatrix = Matrix4x4::RotationAxis(axis, angle);
	Vector3 directVector = rotateMatrix.MuliplyPoint(mPosition - point);
	mPosition = point + directVector;
}

void Transform::LocalToWorld() {
	Matrix4x4 rotation = Matrix4x4::Rotation(mRotation);

	mForworld = rotation.MuliplyPoint(Vector3::forword);
	mRight = rotation.MuliplyPoint(Vector3::right);
	mUp = rotation.MuliplyPoint(Vector3::up);
}

Vector3 Transform::GetPosition() const {
	return mPosition;
}

Quaternion Transform::GetRotation() const {
	return mRotation;
}

Vector3 Transform::GetScaling() const {
	return mScaling;
}

Vector3 Transform::GetForword() const {
	return mForworld;
}

Vector3 Transform::GetRight() const {
	return mRight;
}

Vector3 Transform::GetUp() const {
	return mUp;
}