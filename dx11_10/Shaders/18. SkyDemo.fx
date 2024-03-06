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
	
	//float4 worldPos = mul(input.position, W); << 어짜피 원점이 0,0,0이라 안해도 댐	
	float4 viewPos = mul(float4(input.position.xyz, 0), V);
	//좌표 이동은 무시, 회전만 적용하고 싶은데... >> 4번 행 0으로 밀리면 좌표 변환 없어짐
	//회전 적용 안하면 고개 돌리는데 하늘도 돌아감

	float4 clipPos = mul(viewPos, P);
	
	output.position = clipPos.xyzw;
	output.position.z = output.position.w * 0.999999f;
	//레스터라이저에서 xyz 를 w로 나눠준다... 왜...?
	//그래서 1이 됨...

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