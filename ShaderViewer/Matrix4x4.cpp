#include "Matrix4x4.h"

Matrix4x4::Matrix4x4() : mSIMDMatrix(XMMatrixIdentity()) {

}

XMMATRIX Matrix4x4::GetSIMD() const {
	return mSIMDMatrix;
}

Vector3 Matrix4x4::MuliplyPoint(Vector3 point) {
	XMVECTOR result = XMVector3Transform(point.GetSIMD(), mSIMDMatrix);
	return Vector3(XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result));
}

Matrix4x4 Matrix4x4::LookAt(Vector3 from, Vector3 to, Vector3 up) {
	Matrix4x4 lookAtMatrix;
	lookAtMatrix.mSIMDMatrix = XMMatrixLookAtLH(from.GetSIMD(), to.GetSIMD(), up.GetSIMD());
	return lookAtMatrix;
}

Matrix4x4 Matrix4x4::Perspective(float fov, float aspectRatio, float nearZ, float farZ) {
	Matrix4x4 perspectiveMatrix;
	perspectiveMatrix.mSIMDMatrix = XMMatrixPerspectiveFovLH(fov, aspectRatio, nearZ, farZ);
	return perspectiveMatrix;
}

 Matrix4x4 Matrix4x4::TRS(Vector3 position, Quaternion rotation, Vector3 scaling) {
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(position.GetSIMD());
	XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotation.GetSIMD());
	XMMATRIX scalingMatrix = XMMatrixScalingFromVector(scaling.GetSIMD());

	Matrix4x4 trs;
	trs.mSIMDMatrix = scalingMatrix * rotationMatrix * translationMatrix;
	return trs;
}

Matrix4x4 Matrix4x4::Translation(Vector3 position) {
	Matrix4x4 translationMatrix;
	translationMatrix.mSIMDMatrix = XMMatrixTranslationFromVector(position.GetSIMD());
	return translationMatrix;
}

Matrix4x4 Matrix4x4::Scaling(Vector3 scaling) {
	Matrix4x4 scalingMatrix;
	scalingMatrix.mSIMDMatrix = XMMatrixScalingFromVector(scaling.GetSIMD());;
	return scalingMatrix;
}

Matrix4x4 Matrix4x4::Rotation(Quaternion rotation) {
	Matrix4x4 rotationMatrix;
	rotationMatrix.mSIMDMatrix = XMMatrixRotationQuaternion(rotation.GetSIMD());
	return rotationMatrix;
}

Matrix4x4 Matrix4x4::Rotation(Vector3 eulerAngles) {
	Matrix4x4 rotationMatrix;
	rotationMatrix.mSIMDMatrix = XMMatrixRotationRollPitchYawFromVector(eulerAngles.GetSIMD());
	return rotationMatrix;
}

Matrix4x4 Matrix4x4::RotationAxis(Vector3 axis, float angle) {
	Matrix4x4 rotationMatrix;
	rotationMatrix.mSIMDMatrix = XMMatrixRotationAxis(axis.GetSIMD(), angle);
	return rotationMatrix;
}

const Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	result.mSIMDMatrix = XMMatrixMultiply(m1.mSIMDMatrix, m2.mSIMDMatrix);
	return result;
}