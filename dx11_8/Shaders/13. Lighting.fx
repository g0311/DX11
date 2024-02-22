#include "00. Global.fx"
#include "00. Light.fx"

MeshOutput VS(VertexTextureNormal input)
{	
	MeshOutput output;

	output.position = mul(input.position, W);
	output.worldPosition = output.position.xyz;
	output.position = mul(output.position, VP);
	//VP를 한번에 곱하는게 이득

	output.uv = input.uv;
	output.normal = mul(input.normal, (float3x3)W);
	//translation 고려 x (회전만)

	return output;
}

//Emissive 반사광
//외곽선 구할 때 사용
//림라이트
float4 PS(MeshOutput input) : SV_TARGET
{
	float4 color = ComputeLight(input.normal, input.uv, input.worldPosition);

	return color;
}

technique11 T0
{
	PASS_VP(P0, VS, PS)
};