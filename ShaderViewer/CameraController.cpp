#include "EngineUtility.h"
#include "CameraController.h"

#include <sstream>

CameraController::CameraController(Camera& camera) 
	: mCamera(&camera), mTransform() {
	ZeroMemory(&mCurrPosition, sizeof(POINT));
	ZeroMemory(&mPrevPosition, sizeof(POINT));
}

void CameraController::Start() {
	//mTransform = mCamera->transform;
	mCamera->transform.SetPerent(mTransform);
}

void CameraController::Update(Time tick) {
	RotateCamera(tick);
	MoveCamera(tick);
}

void CameraController::RotateCamera(Time tick) {
	int kRotateX = 0;
	int kRotateY = 0;
	if (Input::IsKeyPressed(VK_LBUTTON)) {
		mCurrPosition = Input::GetMousePosition();
		kRotateX = mCurrPosition.x - mPrevPosition.x;
		kRotateY = mCurrPosition.y - mPrevPosition.y;
		mPrevPosition = mCurrPosition;
	} else {
		mPrevPosition = Input::GetMousePosition();
	}
	mCamera->transform.Rotate(kRotateY * tick.DeltaTime() * 3.0f, 0.0f, 0.0f);
	mTransform.Rotate(0.0f, kRotateX * tick.DeltaTime() * 3.0f, 0.0f);
}

void CameraController::MoveCamera(Time tick) {
	Vector3 movement(0.0f, 0.0f, 0.0f);
	Vector3 forword = mCamera->transform.GetForword();
	Vector3 right = Vector3::Cross(Vector3::up, forword);

	if (Input::IsKeyPressed('W')) {
		movement = movement + forword;
	}
	if (Input::IsKeyPressed('S')) {
		movement = movement - forword;
	}
	if (Input::IsKeyPressed('A')) {
		movement = movement - right;
	}
	if (Input::IsKeyPressed('D')) {
		movement = movement + right;
	}
	mTransform.Translate(movement.Normalize() * 5.0f * tick.DeltaTime());
}