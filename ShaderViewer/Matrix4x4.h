#ifndef _MATRIX4X4_H_
#define _MATRIX4X4_H_

#include "Vector.h"
#include "Quaternion.h"

class Matrix4x4 {
public:
	Matrix4x4();

	static Matrix4x4 LookAt(Vector3 from, Vector3 to, Vector3 up);
	static Matrix4x4 Perspective(float fov, float aspectRatio, float nearZ, float farZ);
	static Matrix4x4 TRS(Vector3 position, Quaternion rotation, Vector3 scaling);

	static Matrix4x4 Translation(Vector3 position);
	static Matrix4x4 Scaling(Vector3 scaling);
	static Matrix4x4 Rotation(Quaternion rotation);
	static Matrix4x4 Rotation(Vector3 eulerAngles);
	static Matrix4x4 RotationAxis(Vector3 axis, float angle);

	friend const Matrix4x4 operator*(const Matrix4x4 &m1, const Matrix4x4 &m2);

	Vector3 MuliplyPoint(Vector3 point);

	XMMATRIX GetSIMD() const;

private:
	XMMATRIX mSIMDMatrix;
};

#endif // !_MATRIX4X4_H_
