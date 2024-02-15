#include "00. Global.fx"

float4 MaterialEmissive; //물체 색상

MeshOutput VS(VertexTextureNormal input)
{	
	MeshOutput output;

	output.position = mul(input.position, W);
	output.worldPosition = output.position;
	output.position = mul(output.position, VP);
	//VP를 한번에 곱하는게 이득

	output.uv = input.uv;
	output.normal = mul(input.normal, (float3x3)W);
	//translation 고려 x (회전만)

	return output;
}

//Emissive 반사광
//외곽선 구할 때 사용
//림라이트
float4 PS(MeshOutput input) : SV_TARGET
{
	float3 cameraPosition = -V._41_42_43;
	//뷰 행렬 : 0,0,0을 카메라 기준 좌표로 변환하는 행렬
	//따라서 뷰 행렬의 41 42 43 좌표는 항상 0을 가르키게 됨... 그냥 잘못 함
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