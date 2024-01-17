
struct VS_INPUT
{
	float4 position : POSITION; //float4를 사용하는 이유: w를 통한 동차저 표기?
	//float4 color : COLOR;
	float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION; //system value (필수적)
	//float4 color : COLOR;
	float2 uv : TEXCOORD;
};

cbuffer CameraData : register(b0)
{
	row_major matrix matView;
	row_major matrix matProjection;
	//row_major?? 행우선?
}

cbuffer TransformData : register(b1)
{
	row_major matrix matWorld;
	//row_major?? 행우선?
}

//IA - <VS> - RS - PS - OM
VS_OUTPUT VS(VS_INPUT input) //c++ 함수랑 똑같음
{
	VS_OUTPUT output;
	
	float4 position = mul(input.position, matWorld); //w
	position = mul(position, matView); //v
	position = mul(position, matProjection); //p
	
	output.position = position;// +offset;//offset 추가 (input 데이터가 변하는 것이 아님)
	output.uv = input.uv;

	return output;
}
//정점 단위 실행
//월드좌표니 로컬좌표니 이런걸 복잡한 수학 공식을 통해 여기서 설정하게 됨
//위치 관련 변환

Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
SamplerState sampler0 : register(s0);
//cpu가 해줌 저수준 작업인듯?

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = texture1.Sample(sampler0, input.uv);
	
	//텍스쳐 0의 uv좌표를 통해 해당 픽셀 색상값 가져오기 
	return color;
}
//모든 픽셀 대상으로 처리되는 단계
//색상 관련 변환