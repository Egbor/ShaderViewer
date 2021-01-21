#include "Engine.h"
#include "Input.h"

#include <sstream>

Engine::Engine(HINSTANCE instance, LPCWSTR className, LPCWSTR wndName, int width, int height, int vpWidth, int vpHeight, int nCmdShow) : 
	Window(instance, className, wndName, width, height, vpWidth, vpHeight, nCmdShow), mMainCamera(), mTime() {
}

void Engine::Run() {
	MSG message;
	ZeroMemory(&message, sizeof(MSG));
	
	mTime.Reset();
	Start();
	while (message.message != WM_QUIT) {
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		} else {
			mTime.Tick();

			if (mMainCamera != NULL) {
				Update();
				Draw();
			}

			Input::Reset();
		}

		//std::stringstream ss;
		//ss << 1.0f / mTime.DeltaTime() << "\n";
		//OutputDebugStringA(ss.str().c_str());
	}
}

void Engine::AddScript(Script &script) {
	mScripts.push_back(&script);
}

void Engine::AddObject(DrawableObject &object) {
	mObjects.push_back(&object);
}

void Engine::DeleteObject(DrawableObject &object) {
	mObjects.remove(&object);
}

void Engine::SetMainCamera(Camera &camera) {
	mMainCamera = &camera;
}

void Engine::Start() {
	for (Script* spt : mScripts) {
		spt->Start();
	}
}

void Engine::Update() {
	mMainCamera->Update(mTime);
	for (Script* spt : mScripts) {
		spt->Update(mTime);
	}
}

void Engine::Draw() {
	ClearBackBuffer();
	for (DrawableObject* obj : mObjects) {
		mMainCamera->Render(*obj);
	}
	SwapBuffers();
}