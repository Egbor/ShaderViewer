#include "ShaderViewer.h"
#include "EngineException.h"

#include "CameraController.h"

#include "Camera.h"
#include "Cube.h"

#include "Model.h"
#include "ObjReader.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmd, int nCmdShow) {
	try {
		ShaderViewer engine(hInstance, L"3DEngine", L"ShaderViewer", 1066, 600, 800, 600, nCmdShow);
		//Camera camera(engine);
		//camera.transform.Translate(0.0f, 0.0f, -10.0f);
		//engine.SetMainCamera(camera);

		//camera.transform.Translate(0.0f, 0.0f, -10.0f);
		////camera.transform.Rotate(0.0f, 0.0f, 0.0f);
		////camera.transform.Scale(1.0f, 1.0f, 1.0f);

		//Shader* vertexShader = new Shader(L"..\\Shaders\\phong.hlsl", "vertex_shader", "vs_4_0");
		//Shader* pixelShader = new Shader(L"..\\Shaders\\phong.hlsl", "pixel_shader", "ps_4_0");

		//Model cloud(engine, camera);
		//Model sword(engine, camera);
		//cloud.SetModelFromFile(L"..\\Models\\Cloud\\Cloud.obj");
		//cloud.SetTextureFromFile(L"..\\Models\\Cloud\\Cloud.png");
		//cloud.SetShaders(*vertexShader, *pixelShader);
		//sword.SetModelFromFile(L"..\\Models\\Cloud\\sword.obj");
		//sword.SetTextureFromFile(L"..\\Models\\Cloud\\sword.png");
		//sword.SetShaders(*vertexShader, *pixelShader);
		///*delete file;*/
		//delete vertexShader;
		//delete pixelShader;

		////cloud.transform.Translate(0.0f, 0.0f, 0.0f);
		////cloud.transform.Rotate(0.0f, 0.0f, 0.0f);
		////cloud.transform.Scale(1.0f, 1.0f, 1.0f);
		////sword.transform.Translate(0.0f, 0.0f, 0.0f);
		////sword.transform.Rotate(0.0f, 0.0f, 0.0f);
		////sword.transform.Scale(1.0f, 1.0f, 1.0f);

		//CameraController CloudController(camera, cloud);
		//CameraController SwordController(camera, sword);

		//engine.AddObject(cloud);
		//engine.AddObject(sword);
		//engine.AddScript(CloudController);
		//engine.AddScript(SwordController);
		//engine.SetMainCamera(camera);
		engine.Run();
	} catch(EngineException &e) {
		MessageBoxW(NULL, e.info().c_str(), L"Error", MB_OK | MB_ICONERROR);
	}
	return 0;
}