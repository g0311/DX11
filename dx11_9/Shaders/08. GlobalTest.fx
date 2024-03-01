#include "00. Global.fx"

VertexOutput VS(VertexTextureNormal input)
{	
	VertexOutput output;

	output.position = mul(input.position, W);
	output.position = mul(output.position, VP);
	//VP�� �ѹ��� ���ϴ°� �̵�

	output.uv = input.uv;
	output.normal = mul(input.normal, (float3x3)W);
	//translation ��� x (ȸ����)

	return output;
}

Texture2D Texture0;
//��� ���� ���ϰ� ����Ʈ �����ؼ� �����ٵ�

//sv target >> �츮�� ������ ����Ÿ�ٿ� �׸��ڴ�
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