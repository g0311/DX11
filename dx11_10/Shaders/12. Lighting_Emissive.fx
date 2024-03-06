#include "00. Global.fx"

float4 MaterialEmissive; //��ü ����

MeshOutput VS(VertexTextureNormal input)
{	
	MeshOutput output;

	output.position = mul(input.position, W);
	output.worldPosition = output.position;
	output.position = mul(output.position, VP);
	//VP�� �ѹ��� ���ϴ°� �̵�

	output.uv = input.uv;
	output.normal = mul(input.normal, (float3x3)W);
	//translation ��� x (ȸ����)

	return output;
}

//Emissive �ݻ籤
//�ܰ��� ���� �� ���
//������Ʈ
float4 PS(MeshOutput input) : SV_TARGET
{
	float3 cameraPosition = -V._41_42_43;
	//�� ��� : 0,0,0�� ī�޶� ���� ��ǥ�� ��ȯ�ϴ� ���
	//���� �� ����� 41 42 43 ��ǥ�� �׻� 0�� ����Ű�� ��... �׳� �߸� ��
	float3 E = normalize(cameraPosition - input.worldPosition);

	float value = saturate(dot(E, input.normal));

	float emissive = 1.0f - value;
	
	emissive = smoothstep(0.0f, 1.0f, emissive);
	emissive - pow(emissive, 2);

	float4 color = MaterialEmissive * emissive;

	return color;
}

technique11 T0
{
	PASS_VP(P0, VS, PS)
};