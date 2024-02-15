#ifndef _LIGHT_FX_
#define _LIGHT_FX_

#include "00. Global.fx"

////////////////
/////Struct/////
////////////////

struct Color
{

};

struct LightDesc
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 emissive;
	float3 direction;
	float padding; //16바이트 정렬 무조건 padding으로
};

struct MaterialDesc
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 emissive;
};

/////////////////////
/////ConstBuffer/////
/////////////////////

cbuffer LightBuffer
{
	LightDesc GlobalLight;
};

cbuffer MaterialBuffer
{
	MaterialDesc Material;
};

/////////////
/////SRV/////
/////////////

Texture2D DiffuseMap;
Texture2D SpecularMap;
Texture2D NormalMap;

//////////////////
/////Function/////
//////////////////

float4 ComputeLight(float3 normal, float2 uv, float3 worldPosition)
{
	float4 ambientColor = 0;
	float4 diffuseColor = 0;
	float4 specularColor = 0;
	float4 emissiveColor = 0;

	//Ambient
	{
		float4 color = GlobalLight.ambient * Material.ambient;
		ambientColor = DiffuseMap.Sample(LinearSampler, uv) * color;
	}
	//diffuse
	{
		float4 color = DiffuseMap.Sample(LinearSampler, uv);
		float value = dot(-GlobalLight.direction, normalize(normal));
		diffuseColor = color * value * GlobalLight.diffuse * Material.diffuse;
	}
	//specular
	{
		//float3 R = reflect(GlobalLight.direction, input.normal);
		float3 R = GlobalLight.direction - (2 * normal * dot(GlobalLight.direction, normal));
		R = normalize(R);

		float3 cameraPosition = CameraPosition();
		float3 E = normalize(cameraPosition - worldPosition);

		float value = saturate(dot(R, E)); //clamp(0~1)
		float specular = pow(value, 10);

		specularColor = GlobalLight.specular * Material.specular * specular;
	}
	//emissive
	{
		float3 cameraPosition = CameraPosition();
		float3 E = normalize(cameraPosition - worldPosition);

		float value = saturate(dot(E, normal));

		float emissive = 1.0f - value;

		emissive = smoothstep(0.0f, 1.0f, emissive);
		emissive - pow(emissive, 2);

		emissiveColor = GlobalLight.emissive * Material.emissive * emissive;
	}
	return ambientColor + diffuseColor + specularColor + emissiveColor;
}

void ComputeNormalMapping(inout float3 normal, float3 tangent, float2 uv) //inout >> 아웃매개변수
{
	//[0, 255] >> [0, 1] 변환
	float4 map = NormalMap.Sample(LinearSampler, uv);
	if (any(map.rgb) == false) //000이라 정보 없을 때
		return;
	
	float3 T = normalize(tangent);//x
	float3 N = normalize(normal);//z
	float3 B = normalize(cross(N, T));//y
	float3x3 TBN = float3x3(T, B, N); //탄젠트 >> 월드 스페이스로 가는 변환행렬
	
	//[0, 1] >> [-1, 1] 변환
	float3 tangentNormalVector = (map.rgb * 2.0f - 1.0f);
	
	float3 worldNormal = mul(tangentNormalVector, TBN); //방향(벡터)가 필요해서 3x3 행렬로 처리 가능
	normal = worldNormal;
}
#endif