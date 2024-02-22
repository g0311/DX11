#include "00. Global.fx"
#include "00. Light.fx"

#define MAX_MODEL_TRANSFORMS 50

cbuffer BoneBuffer
{
	matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
};

uint BoneIndex;

MeshOutput VS(VertexTextureNormalTangent input)
{	
	MeshOutput output;

	output.position = mul(input.position, BoneTransforms[BoneIndex]);
	//여기서 로컬 좌표계로 (루트 좌표계)변환 됨
	output.position = mul(output.position, W);
	output.worldPosition = output.position.xyz;
	output.position = mul(output.position, VP);
	//VP를 한번에 곱하는게 이득

	output.uv = input.uv;
	output.normal = mul(input.normal, (float3x3)W);
	//translation 고려 x (회전만)
	output.tangent = mul(input.tangent, (float3x3)W);

	return output;
}

float4 PS(MeshOutput input) : SV_TARGET
{
	//ComputeNormalMapping(input.normal, input.tangent, input.uv);

	//float4 color = ComputeLight(input.normal, input.uv, input.worldPosition);

	float4 color = DiffuseMap.Sample(LinearSampler, input.uv);

	return color;
}

float4 PS_RED(MeshOutput input) : SV_TARGET
{
	return float4(1.f, 0.f, 0.f, 1.f);
}

technique11 T0
{
	PASS_VP(P0, VS, PS)
	PASS_RS_VP(P1, FillModeWireFrame, VS, PS_RED)
};