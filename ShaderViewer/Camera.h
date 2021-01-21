#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Object.h"
#include "Matrix4x4.h"

class DrawableObject;

class Camera : public Object {
public:
	Camera(Engine &engine);

	void Update(Time tick);
	void Render(DrawableObject &object);

	float GetFOV() const;
	float GetAspectRatio() const;
	float GetNearZ() const;
	float GetFarZ() const;

private:
	void UpdateConstBuffer(DrawableObject &object);

	Vector3 mFocusDir;
	Vector3 mUpDir;
	Vector3 mPosition;

	float mFOV;
	float mAspectRatio;
	float mNearZ;
	float mFarZ;
};

#endif // !_CAMERA_H_
