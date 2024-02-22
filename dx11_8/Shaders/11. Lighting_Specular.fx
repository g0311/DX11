#include "00. Global.fx"

float3 LightDir;
float4 LightSpecular; //빛 색상
float4 MaterialSpecular; //물체 색상

Texture2D diffuseMap; //텍스쳐 맵이라기 보단 디퓨즈 맵이라고 하는게 맞음

MeshOutput VS(VertexTextureNormal input)
{	
	MeshOutput output;

	output.position = mul(input.position, W);
	output.worldPosition = input.position;
	output.position = mul(output.position, VP);
	//VP를 한번에 곱하는게 이득

	output.uv = input.uv;
	output.normal = mul(input.normal, (float3x3)W);
	//translation 고려 x (회전만)

	return output;
}

//Specular 반사광
//한 방향으로 완전히 반사되는 빛 (phong)
float4 PS(MeshOutput input) : SV_TARGET
{
	//float3 R = reflect(LightDir, input.normal);
	float3 R = LightDir - (2 * input.normal * dot(LightDir, input.normal));
	R = normalize(R);

	float3 cameraPosition = -V._41_42_43;
	float3 E = normalize(cameraPosition - input.worldPosition);

	float value = saturate(dot(R, E)); //clamp(0~1)
	float specular = pow(value, 10);

	float4 color = LightSpecular * MaterialSpecular * specular;
	return color;
}

technique11 T0
{
	PASS_VP(P0, VS, PS)
};