#ifndef _TRANSFORM_H_
#define _TRANSFOMR_H_

#include "Quaternion.h"
#include "Vector.h"

class TransformFixed {
public:
	TransformFixed();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	void SetPosition(Vector3 position);
	void SetRotation(Vector3 eulerAngles);
	void SetScale(Vector3 scale);

	void SetPerent(TransformFixed &transform);

	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void Scale(float x, float y, float z);

	void Translate(Vector3 posOffset);
	void Rotate(Vector3 rotOffset);
	void Scale(Vector3 sclOffset);

	void Rotate(Vector3 axis, float angle);

	Quaternion GetRotation() const;
	Vector3 GetPosition() const;
	Vector3 GetScale() const;

	Vector3 GetForword() const;
	Vector3 GetUp() const;
	Vector3 GetRight() const;

private:
	//Quaternion UpdateRotation();
	//Vector3 UpdatePosition();
	//Vector3 UpdateScale();

	static TransformFixed world;

protected:
	TransformFixed* mPerent;

	Quaternion mLocalRotation;
	Vector3 mLocalPosition;
	Vector3 mLocalScale;
};

#endif // !_TRANSFORM_H_
