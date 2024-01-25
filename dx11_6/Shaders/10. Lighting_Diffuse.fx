#include "00. Global.fx"

float3 LightDir;
float4 LightDiffuse; //빛 색상
float4 MaterialDiffuse; //물체 색상
Texture2D diffuseMap; //텍스쳐 맵이라기 보단 디퓨즈 맵이라고 하는게 맞음

VertexOutput VS(VertexTextureNormal input)
{	
	VertexOutput output;

	output.position = mul(input.position, W);
	output.position = mul(output.position, VP);
	//VP를 한번에 곱하는게 이득

	output.uv = input.uv;
	output.normal = mul(input.normal, (float3x3)W);
	//translation 고려 x (회전만)

	return output;
}

//Diffuse 분산 광
//물체의 표면에서 분산되어 눈으로 바로 들어오는 빛
//각도에 따라 밝기가 다르다 (람바르트 공식) 
float4 PS(VertexOutput input) : SV_TARGET
{
	float4 color = diffuseMap.Sample(LinearSampler, input.uv);
	
	float value = dot(-LightDir, normalize(input.normal));
	color = color * value * LightDiffuse * MaterialDiffuse;

	return color;
}

technique11 T0
{
	PASS_VP(P0, VS, PS)

	pass P1
	{
		SetRasterizerState(FillModeWireFrame);

		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};