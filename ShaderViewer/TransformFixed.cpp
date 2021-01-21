#include "TransformFixed.h"

TransformFixed TransformFixed::world;

TransformFixed::TransformFixed()
	: mLocalPosition(0.0f, 0.0f, 0.0f), mLocalRotation(0.0f, 0.0f, 0.0f, 1.0f), mLocalScale(1.0f, 1.0f, 1.0f), mPerent(&world) {
	world.SetScale(0.0f, 0.0f, 0.0f);
}

void TransformFixed::SetPosition(float x, float y, float z) {
	SetPosition(Vector3(x, y, z));
}

void TransformFixed::SetRotation(float x, float y, float z) {
	SetRotation(Vector3(x, y, z));
}

void TransformFixed::SetScale(float x, float y, float z) {
	SetScale(Vector3(x, y, z));
}

void TransformFixed::SetPosition(Vector3 position) {
	mLocalPosition = position;
}

void TransformFixed::SetRotation(Vector3 eulerAngles) {
	Quaternion rotOffset = Quaternion(eulerAngles);
	mLocalRotation = rotOffset;
}

void TransformFixed::SetScale(Vector3 scale) {
	mLocalScale = scale;
}

void TransformFixed::SetPerent(TransformFixed& transform) {
	mPerent = &transform;
}

Vector3 TransformFixed::GetPosition() const {
	return mLocalPosition + mPerent->mLocalPosition;
}

Vector3 TransformFixed::GetScale() const {
	return mLocalScale + mPerent->mLocalScale;
}

Quaternion TransformFixed::GetRotation() const {
	return mLocalRotation * mPerent->mLocalRotation;
}

void TransformFixed::Translate(float x, float y, float z) {
	Translate(Vector3(x, y, z));
}

void TransformFixed::Rotate(float x, float y, float z) {
	Rotate(Vector3(x, y, z));
}

void TransformFixed::Scale(float x, float y, float z) {
	Scale(Vector3(x, y, z));
}

void TransformFixed::Translate(Vector3 posOffset) {
	mLocalPosition = mLocalPosition + posOffset;
}

void TransformFixed::Rotate(Vector3 rotOffset) {
	Quaternion qRotOffset = Quaternion(rotOffset);
	mLocalRotation = qRotOffset * mLocalRotation;
}

void TransformFixed::Scale(Vector3 sclOffset) {
	mLocalScale = mLocalScale + sclOffset;
}

void TransformFixed::Rotate(Vector3 axis, float angle) {
	Quaternion rotOffset = Quaternion(axis, angle);
	mLocalRotation = rotOffset * mLocalRotation;
}

Vector3 TransformFixed::GetForword() const {
	return Quaternion::RotatePoint(GetRotation(), Vector3(0.0f, 0.0f, 1.0f));//mRotation.GetLocalForword();
}

Vector3 TransformFixed::GetUp() const {
	return Quaternion::RotatePoint(GetRotation(), Vector3(0.0f, 1.0f, 0.0f));
}

Vector3 TransformFixed::GetRight() const {
	return Quaternion::RotatePoint(GetRotation(), Vector3(1.0f, 0.0f, 0.0f));
}