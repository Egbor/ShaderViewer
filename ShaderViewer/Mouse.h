#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <Windows.h>

enum MOUSE_KEY {

};

class Mouse {
public:
	Mouse();

	BOOL IsButtonPressed();
	BOOL IsButtonReleased();

	POINT GetCursorPosition() const;

private:
	POINT mPositionRelativeWindow;
};

#endif // !_MOUSE_H_
