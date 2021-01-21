#ifndef _CAMERA_CONTROLLER_H_
#define _CAMERA_CONTROLLER_H_

#include "Script.h"
#include "Camera.h"

//class Object;
//class Camera;
//class TransformFixed;

class CameraController : public Script {
public:
	CameraController(Camera &camera);

	void Start() override;
	void Update(Time tick) override;

private:
	void RotateCamera(Time tick);
	void MoveCamera(Time tick);

	POINT mCurrPosition;
	POINT mPrevPosition;

	Camera* mCamera;
	TransformFixed mTransform;
};

#endif // !_CAMERA_CONTROLLER_H_
