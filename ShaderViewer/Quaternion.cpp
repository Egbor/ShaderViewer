#include "Quaternion.h"
#include "Matrix4x4.h"
#include <cmath>

Quaternion::Quaternion(float x, float y, float z, float w) : mEulerAngles(0.0f, 0.0f, 0.0f) {
	Set(x, y, z, w);
}

Quaternion::Quaternion(float eulerX, float eulerY, float eulerZ) : mEulerAngles(eulerX, eulerY, eulerZ) {
	mSMIDQuaternion = XMQuaternionRotationRollPitchYaw(eulerX, eulerY, eulerZ);
}

Quaternion::Quaternion(Vector3 eulerAngles) : mEulerAngles(eulerAngles) {
	mSMIDQuaternion = XMQuaternionRotationRollPitchYawFromVector(eulerAngles.GetSIMD());
}

Quaternion::Quaternion(Vector3 axis, float angle) : mEulerAngles(0.0f, 0.0f, 0.0f) {
	mSMIDQuaternion = XMQuaternionRotationAxis(axis.GetSIMD(), angle);
}

Quaternion::Quaternion(XMVECTOR quaternion) : mEulerAngles(0.0f, 0.0f, 0.0f), mSMIDQuaternion(quaternion) {

}

XMVECTOR Quaternion::GetSIMD() const {
	return mSMIDQuaternion;
}

Vector3 Quaternion::ToEulerAngles()const {
	return Quaternion::ToEulerAngles(XMVectorGetX(mSMIDQuaternion), XMVectorGetY(mSMIDQuaternion), XMVectorGetZ(mSMIDQuaternion), XMVectorGetW(mSMIDQuaternion));
}

void Quaternion::Set(float x, float y, float z, float w) {
	mSMIDQuaternion = XMVectorSet(x, y, z, w);;
	mEulerAngles = ToEulerAngles(x, y, z, w);
}

Quaternion Quaternion::AxisAngle(Vector3 axis, float angle) {
	XMVECTOR quaternion = XMQuaternionRotationAxis(axis.GetSIMD(), angle);
	return Quaternion(XMVectorGetX(quaternion), XMVectorGetY(quaternion), XMVectorGetZ(quaternion), XMVectorGetW(quaternion));
}

Quaternion Quaternion::Euler(float x, float y, float z) {
	XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(x, y, z);
	return Quaternion(XMVectorGetX(quaternion), XMVectorGetY(quaternion), XMVectorGetZ(quaternion), XMVectorGetW(quaternion));
}

Quaternion Quaternion::Euler(Vector3 eulerAngles) {
	XMVECTOR quaternion = XMQuaternionRotationRollPitchYawFromVector(eulerAngles.GetSIMD());
	return Quaternion(XMVectorGetX(quaternion), XMVectorGetY(quaternion), XMVectorGetZ(quaternion), XMVectorGetW(quaternion));
}

Vector3 Quaternion::RotatePoint(Quaternion rot, Vector3 point) {
	XMVECTOR qrot = rot.mSMIDQuaternion;
	XMVECTOR _qrot = XMQuaternionInverse(rot.mSMIDQuaternion);
	XMVECTOR qpoint = point.GetSIMD();
	qpoint = XMQuaternionMultiply(XMQuaternionMultiply(_qrot, qpoint), qrot);
	return Vector3(XMVectorGetX(qpoint), XMVectorGetY(qpoint), XMVectorGetZ(qpoint));
}

Vector3 Quaternion::GetLocalForword() const {
	XMMATRIX rotation = XMMatrixRotationQuaternion(mSMIDQuaternion);
	XMVECTOR forword = rotation.r[2];
	return Vector3(XMVectorGetX(forword), XMVectorGetY(forword), XMVectorGetZ(forword));
}

Vector3 Quaternion::GetLocalUp() const {
	XMMATRIX rotation = XMMatrixRotationQuaternion(mSMIDQuaternion);
	XMVECTOR up = rotation.r[1];
	return Vector3(XMVectorGetX(up), XMVectorGetY(up), XMVectorGetZ(up));
}

Vector3 Quaternion::GetLocalRight() const {
	XMMATRIX rotation = XMMatrixRotationQuaternion(mSMIDQuaternion);
	XMVECTOR right = rotation.r[0];
	return Vector3(XMVectorGetX(right), XMVectorGetY(right), XMVectorGetZ(right));
}

inline Quaternion Quaternion::Inverse() {
	return Quaternion(XMQuaternionInverse(mSMIDQuaternion));
}

Quaternion Quaternion::LookRotate(Vector3& to, Vector3& up) {
	XMVECTOR axisUp = XMVector3Normalize(up.GetSIMD());
	XMVECTOR axisZ = XMVector3Normalize(to.GetSIMD());
	XMVECTOR axisX = XMVector3Normalize(XMVector3Cross(axisUp, axisZ));
	XMVECTOR axisY = XMVector3Normalize(XMVector3Cross(axisZ, axisX));
	XMVECTOR zero = XMVectorZero();
	XMMATRIX rotation(axisX, axisY, axisZ, zero);
	
	XMVECTOR quaternion = XMQuaternionRotationMatrix(rotation);
	return Quaternion(XMVectorGetX(quaternion), XMVectorGetY(quaternion), XMVectorGetZ(quaternion), XMVectorGetW(quaternion));
}

Vector3 Quaternion::ToEulerAngles(float x, float y, float z, float w) {
	float roll;
	float pitch;
	float yaw;

	float sinrcosp = 2 * (w * x + y * z);
	float cosrcosp = 1 - 2 * (x * x + y * y);
	roll = std::atan2f(sinrcosp, cosrcosp);

	float sinp = 2 * (w * y - z * x);
	if (std::abs(sinp) >= 1) {
		pitch = std::copysignf(XM_PI / 2, sinp);
	} else {
		pitch = std::asinf(sinp);
	}

	float sinycosp = 2 * (w * z + x * y);
	float cosycosp = 1 - 2 * (y * y + z * z);
	yaw = std::atan2f(sinycosp, cosycosp);

	//float test = x * y + z * w;
	//if (test > 0.499f) {
	//	roll = 2.0f * std::atan2f(x, w);
	//	pitch = XM_PI / 2.0f;
	//	yaw = 0.0f;
	//	return Vector3(roll, pitch, yaw);
	//} 
	//if (test < -0.499f) {
	//	roll = -2.0f * std::atan2f(x, w);
	//	pitch = -XM_PI / 2.0f;
	//	yaw = 0.0f;
	//	return Vector3(roll, pitch, yaw);

	//}
	//roll = atan2f(2.0f * y * w - 2.0f * x * z, 1.0f - 2.0f * y * y - 2.0f * z * z);
	//pitch = asinf(2 * test);
	//yaw = atan2(2.0f * x * w - 2.0f * y * z, 1.0f - 2.0f * x * x - 2.0f * z * z);
	return Vector3(roll, pitch, yaw);
}

Vector3 Quaternion::ToVector() const {
	return Vector3(XMVectorGetX(mSMIDQuaternion), XMVectorGetY(mSMIDQuaternion), XMVectorGetZ(mSMIDQuaternion));
}

Quaternion operator*(Quaternion q1, Quaternion q2) {
	XMVECTOR quaternion = XMQuaternionMultiply(q1.mSMIDQuaternion, q2.mSMIDQuaternion);
	return Quaternion(XMVectorGetX(quaternion), XMVectorGetY(quaternion), XMVectorGetZ(quaternion), XMVectorGetW(quaternion));
}