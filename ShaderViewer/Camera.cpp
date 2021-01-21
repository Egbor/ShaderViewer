#include "Engine.h"

#include "DrawableObject.h"
#include "Camera.h"
#include "Matrix4x4.h"

Camera::Camera(Engine &engine) : Object(engine), mFOV(DEGREES_TO_RADIANS(60.0)), mNearZ(1.0f), mFarZ(1000.0f), 
	mFocusDir(0.0f, 0.0f, 1.0f), mUpDir(0.0f, 1.0f, 0.0f), mPosition(0.0f, 0.0f, 0.0f) {
	mAspectRatio = (float)mEngine->GetViewportWidth() / (float)mEngine->GetViewportHeight();
}

float Camera::GetFOV() const {
	return mFOV;
}

float Camera::GetAspectRatio() const {
	return mAspectRatio;
}

float Camera::GetNearZ() const {
	return mNearZ;
}

float Camera::GetFarZ() const {
	return mFarZ;
}

void Camera::Update(Time tick) {
	mFocusDir = transform.GetForword();
	mUpDir = transform.GetUp();
	mPosition = transform.GetPosition();
}

void Camera::Render(DrawableObject& object) {
	UpdateConstBuffer(object);
	object.Draw();
}

void Camera::UpdateConstBuffer(DrawableObject &object) {
	Vector3 objPosition = object.transform.GetPosition();
	Vector3 objScale = object.transform.GetScale();
	Quaternion objRotation = object.transform.GetRotation();

	Matrix4x4 world = Matrix4x4::TRS(objPosition, objRotation, objScale);
	Matrix4x4 view = Matrix4x4::LookAt(mPosition, mPosition + mFocusDir, mUpDir);
	Matrix4x4 projection = Matrix4x4::Perspective(mFOV, mAspectRatio, mNearZ, mFarZ);
	Matrix4x4 wvp = world * view * projection;

	WorldViewProjection wvpBuffer;
	XMStoreFloat4x4(&wvpBuffer.mWorldViewProjection, XMMatrixTranspose(wvp.GetSIMD()));
	XMStoreFloat4x4(&wvpBuffer.mWorld, world.GetSIMD());
	XMStoreFloat4x4(&wvpBuffer.mView, view.GetSIMD());
	XMStoreFloat4x4(&wvpBuffer.mProjection, projection.GetSIMD());

	ID3D11DeviceContext* context = mEngine->GetContext();
	ID3D11Buffer* wvpCBuffer = mEngine->GetWVPCBuffer();
	context->UpdateSubresource(wvpCBuffer, 0, NULL, &wvpBuffer, 0, 0);
}