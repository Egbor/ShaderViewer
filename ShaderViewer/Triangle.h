#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "Shader.h"
#include "DrawableObject.h"

class Triangle : public DrawableObject {
public:
	Triangle(Engine &engine, Camera &camera);
	~Triangle();

	virtual void Draw();

private:
	Shader* vertexShader;
	Shader* pixelShader;
};

#endif // !_TRIANGLE_H_
