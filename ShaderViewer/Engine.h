#ifndef _ENGINE_H_
#define _ENGINE_H

#include "Time.h"
#include "Script.h"
#include "DrawableObject.h"
#include "Camera.h"
#include "Window.h"

#include <list>

class Engine : public Window {
public:
	Engine(HINSTANCE instance, LPCWSTR className, LPCWSTR wndName, int width, int height, int vpWidth, int vpHeight, int nCmdShow);

	void Run();
	void AddScript(Script &script);
	void AddObject(DrawableObject &object);
	void DeleteObject(DrawableObject &object);
	void SetMainCamera(Camera &camera);

private:
	void Start();
	void Update();
	void Draw();

	std::list<Script*> mScripts;
	std::list<DrawableObject*> mObjects;

	Camera* mMainCamera;
	Time mTime;
};

#endif // !_ENGINE_H_
