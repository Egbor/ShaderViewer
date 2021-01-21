#ifndef _CUBE_H_
#define _CUBE_H_

#include "DrawableObject.h"

class Cube : public DrawableObject {
public:
	Cube(Engine &engine, Camera &camera);
	~Cube();

	virtual void Draw();

private:
	Shader* vertexShader;
	Shader* pixelShader;
};

#endif // !_CUBE_H_
