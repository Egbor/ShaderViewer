cbuffer CBufferPerFrame {
	static float4 AmbientColor = {1.0f, 1.0f, 1.0f, 1.0f};
	static float4 LightColor = {1.0f, 1.0f, 1.0f, 1.0f};
	static float3 LightDirection = {0.0f, 1.0f, 0.0f};
	static float3 CameraPosition = {0.0f, 0.0f, -10.0f};
	static float4 SpecularColor = {1.0f, 1.0f, 1.0f, 1.0f};
	static float SpecularPower  = {25.0f};
	static float KAmbientLighting = {0.4f};
	static float KDiffuseLighting  = {1.0f};
	static float KSpecularLighting  = {1.0f};
};

cbuffer WorldViewProjection : register(b0) {
	float4x4 WorldViewProjection;
	float4x4 World;
};

struct VS_INPUT {
	float4 ObjectPosition : POSITION;
	float3 TextureCoordinate : TEXCOORD;
	float3 Normal : NORMAL;	
};

struct VS_OUTPUT {
	float4 Position : SV_Position;
	float3 Normal : NORMAL;
	float2 TextureCoordinate : TEXCOORD0;	
	float3 LightDirection : TEXCOORD1;
	float3 ViewDirection : TEXCOORD2;
};

SamplerState ColorSampler {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;	
};

Texture2D ColorTexture : register(t0);

VS_OUTPUT vertex_shader(VS_INPUT IN) {
	VS_OUTPUT OUT = (VS_OUTPUT)0;
	OUT.Position = mul(IN.ObjectPosition, WorldViewProjection);
	OUT.TextureCoordinate = float2(IN.TextureCoordinate.x, 1.0f - IN.TextureCoordinate.y);
	OUT.Normal = normalize(mul(float4(IN.Normal, 0), transpose(World)).xyz);
	OUT.LightDirection = normalize(-LightDirection);
	float3 worldPosition = mul(IN.ObjectPosition, transpose(World)).xyz;
	OUT.ViewDirection = normalize(CameraPosition - worldPosition);
	return OUT;
}

float4 pixel_shader(VS_OUTPUT IN) : SV_Target {
	float4 OUT = (float4)0;
	float3 normal = normalize(IN.Normal);
	float3 lightDirection = normalize(IN.LightDirection);
	float3 viewDirection = normalize(IN.ViewDirection);
	float n_dot_l = dot(lightDirection, normal);
	float4 color = ColorTexture.Sample(ColorSampler, IN.TextureCoordinate);
	if ((color.r == 0.0f) && (color.g == 0.0f) && (color.b == 0.0f)) {
		color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	float3 ambient = AmbientColor.rgb * AmbientColor.a * color.rgb;
	float3 diffuse = (float3)0;
	float3 specular = (float3)0;
	if (n_dot_l > 0) {
		diffuse = LightColor.rgb * LightColor.a * saturate(n_dot_l) * color.rgb;
		float3 reflectionVector = normalize(2 * n_dot_l * normal - lightDirection);
		specular = SpecularColor.rgb * SpecularColor.a * min(pow(saturate(dot(reflectionVector, viewDirection)), SpecularPower), color.w);
	}
	OUT.rgb = KAmbientLighting * ambient + KDiffuseLighting * diffuse + KSpecularLighting * specular;
	OUT.a = 1.0f;
	return OUT;
}