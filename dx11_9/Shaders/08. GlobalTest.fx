#include "00. Global.fx"

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

//sv target >> 우리가 지정한 렌더타겟에 그리겠다
float4 PS(VertexOutput input) : SV_TARGET
{
	//float3 normal = normalize(input.normal);
	//float3 light = -LightDir;
	//return float4(1,1,1,1) * dot(light, normal);
	return Texture0.Sample(LinearSampler, input.uv);// * float4(1, 1, 1, 1) * dot(light, normal);
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