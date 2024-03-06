#include "00. Global.fx"
#include "00. Light.fx"

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_OUT VS(VertexTextureNormalTangent input)
{	
	VS_OUT output;
	
	//float4 worldPos = mul(input.position, W); << ��¥�� ������ 0,0,0�̶� ���ص� ��	
	float4 viewPos = mul(float4(input.position.xyz, 0), V);
	//��ǥ �̵��� ����, ȸ���� �����ϰ� ������... >> 4�� �� 0���� �и��� ��ǥ ��ȯ ������
	//ȸ�� ���� ���ϸ� �� �����µ� �ϴõ� ���ư�

	float4 clipPos = mul(viewPos, P);
	
	output.position = clipPos.xyzw;
	output.position.z = output.position.w * 0.999999f;
	//�����Ͷ��������� xyz �� w�� �����ش�... ��...?
	//�׷��� 1�� ��...

	output.uv = input.uv;

	return output;
}

float4 PS(VS_OUT input) : SV_TARGET
{
	float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
	return color;
}

technique11 T0
{
	pass P0
	{
		SetRasterizerState(FrontCounterClockwiseTrue);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};