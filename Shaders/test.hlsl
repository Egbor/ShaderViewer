cbuffer WorldViewProjection : register(b0) {
	float4x4 WorldViewProjection;
};

struct VS_INPUT {
	float4 ObjectPosition : POSITION;
	float3 TextureCoordinate : TEXCOORD;
	float3 Normal : NORMAL;	
};

struct VS_OUTPUT {
	float4 Position : SV_Position;
	float3 Color : COLOR;
};

VS_OUTPUT vertex_shader(VS_INPUT IN) {
	VS_OUTPUT OUT = (VS_OUTPUT)0;
	OUT.Position = IN.ObjectPosition * 0.1f;
	OUT.Color = float3(1.0f, 0.0f, 0.0f);
	return OUT;
}

float3 pixel_shader(VS_OUTPUT IN) : SV_Target {
	return IN.Color;
}