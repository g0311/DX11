
struct VS_INPUT
{
	float4 position : POSITION; //float4�� ����ϴ� ����: w�� ���� ������ ǥ��?
	//float4 color : COLOR;
	float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION; //system value (�ʼ���)
	//float4 color : COLOR;
	float2 uv : TEXCOORD;
};

cbuffer TransformData : register(b0)
{
	float4 offset;
}

//IA - <VS> - RS - PS - OM
VS_OUTPUT VS(VS_INPUT input) //c++ �Լ��� �Ȱ���
{
	VS_OUTPUT output;
	output.position = input.position + offset;//offset �߰� (input �����Ͱ� ���ϴ� ���� �ƴ�)
	output.uv = input.uv;

	return output;
}
//���� ���� ����
//������ǥ�� ������ǥ�� �̷��� ������ ���� ������ ���� ���⼭ �����ϰ� ��
//��ġ ���� ��ȯ

Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
SamplerState sampler0 : register(s0);
//cpu�� ���� ������ �۾��ε�?

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = texture0.Sample(sampler0, input.uv);
	
	//�ؽ��� 0�� uv��ǥ�� ���� �ش� �ȼ� ���� �������� 
	return color;
}
//��� �ȼ� ������� ó���Ǵ� �ܰ�
//���� ���� ��ȯ