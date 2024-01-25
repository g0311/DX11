#include "00. Global.fx"

float4 LightAmbient; //조명의 앰비언트 효과 (태양의 빛 색상 자체)
float4 MaterialAmbient; //물체마다 흡수하는 색상 (빨간색 << 흰색 >>>>> 빨간색 표현)
//둘이 합쳐진다 느낌

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

Texture2D Texture0;
//얘는 지금 못하고 라이트 관련해서 묶어줄듯

//Ambient 환경(주변) 광
//수많은 반사를 거쳐서 광원이 불분명한 빛
//일정한 밝기와 색으로 표현됨 
float4 PS(VertexOutput input) : SV_TARGET
{
	float4 color = LightAmbient * MaterialAmbient;
	//return color;
	return Texture0.Sample(LinearSampler, input.uv) * color;
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