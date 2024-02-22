#include "00. Global.fx"

float3 LightDir;
float4 LightDiffuse; //�� ����
float4 MaterialDiffuse; //��ü ����
Texture2D diffuseMap; //�ؽ��� ���̶�� ���� ��ǻ�� ���̶�� �ϴ°� ����

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

//Diffuse �л� ��
//��ü�� ǥ�鿡�� �л�Ǿ� ������ �ٷ� ������ ��
//������ ���� ��Ⱑ �ٸ��� (���ٸ�Ʈ ����) 
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