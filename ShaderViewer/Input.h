#ifndef _INPUT_H_
#define _INPUT_H_

#include <Windows.h>

enum KeyStatus { Up, Down };

class Input {
public:
	Input() = default;

	static BOOL IsKeyPressed(int key);
	static BOOL IsKeyReleased(int key);

	static POINT GetMousePosition();
	static void Reset();

	static float mouseScrollDelta;
private:
	static KeyStatus mPressedKeyTable[256];
};

#endif // !_INPUT_H_
