#include "00. Global.fx"

float4 LightAmbient; //������ �ں��Ʈ ȿ�� (�¾��� �� ���� ��ü)
float4 MaterialAmbient; //��ü���� ����ϴ� ���� (������ << ��� >>>>> ������ ǥ��)
//���� �������� ����

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

//Ambient ȯ��(�ֺ�) ��
//������ �ݻ縦 ���ļ� ������ �Һи��� ��
//������ ���� ������ ǥ���� 
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