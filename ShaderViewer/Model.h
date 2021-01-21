#ifndef _MODEL_H_
#define _MODEL_H_

#include "DrawableObject.h"

class Model : public DrawableObject {
	using Super = DrawableObject;
public:
	Model(Engine& engine, Camera& camera);

	virtual void Draw();

	void SetShaders(Shader& vertexShader, Shader& pixelShader);
	void SetModelFromFile(LPCWSTR filename);
	void SetTextureFromFile(LPCWSTR filename);

private:
	//ComPtr<ID3D11ShaderResourceView> mTextureView;
};

#endif // !_MODEL_H_
