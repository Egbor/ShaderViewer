#include "Input.h"

KeyStatus Input::mPressedKeyTable[256] = { Up };

float Input::mouseScrollDelta = 0.0f;

void Input::Reset() {
	mouseScrollDelta = 0.0f;
}

BOOL Input::IsKeyPressed(int key) {
	return (GetAsyncKeyState(key) & 0x8000);
}

BOOL Input::IsKeyReleased(int key) {
	if ((GetAsyncKeyState(key) & 0x8000)) {
		mPressedKeyTable[key] = Down;
	} else if (mPressedKeyTable[key] == Down) {
		mPressedKeyTable[key] = Up;
		return TRUE;
	}
	return FALSE;
}

POINT Input::GetMousePosition() {
	POINT point;
	GetCursorPos(&point);
	return point;
}