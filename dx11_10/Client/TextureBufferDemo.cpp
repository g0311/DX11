#include "pch.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"
#include "Light.h"
#include "TextureBufferDemo.h"
#include "TextureBuffer.h"

void TextureBufferDemo::Init()
{
	auto newSrv = MakeComputeShaderTexture();

	_shader = make_shared<Shader>(L"23. RenderDemo.fx");

	//camera
	{
		auto camera = make_shared<GameObject>();
		camera->GetOrAddTransform()->SetPosition(Vec3{ 0.f, 0.f, -5.f });
		camera->AddComponent(make_shared<Camera>());
		camera->AddComponent(make_shared<CameraScript>());
		
		CUR_SCENE->Add(camera);
	}
	//Light
	{
		auto light = make_shared<GameObject>();
		light->GetOrAddTransform()->SetPosition(Vec3{ 0.f, 0.f, -5.f });
		light->AddComponent(make_shared<Light>());

		LightDesc lightdesc;
		lightdesc.ambient = Vec4(0.4f);
		lightdesc.diffuse = Vec4(1.f);
		lightdesc.specular = Vec4(0.1f);
		lightdesc.direction = Vec3(1.f, 0.f, 1.f);

		light->GetLight()->SetLightDesc(lightdesc);

		CUR_SCENE->Add(light);
	}

	//Objs
	{
		{
			//Material
			{
				shared_ptr<Material> material = make_shared<Material>();
				material->SetShader(_shader);
				auto texture = make_shared<Texture>();
				texture->SetSRV(newSrv);
				material->SetDiffuseMap(texture);
				MaterialDesc& desc = material->GetMaterialDesc();
				desc.ambient = Vec4(1.f);
				desc.diffuse = Vec4(1.f);
				desc.specular = Vec4(1.f);
				RESOURCES->Add(L"Veigar", material);
			}

			for (int32 i = 0; i < 100; i++)
			{
				auto obj = make_shared<GameObject>();
				obj->GetOrAddTransform()->SetPosition(Vec3{ (float)(rand() % 100), 0, (float)(rand() % 100) });
				obj->AddComponent(make_shared<MeshRenderer>());
				{
					obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Veigar"));
				}
				{
					auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
					obj->GetMeshRenderer()->SetMesh(mesh);
					obj->GetMeshRenderer()->SetPass(0);
				}
				CUR_SCENE->Add(obj);
			}
		}
	}
	RENDER->Init(_shader);
}

void TextureBufferDemo::Update()
{
}

void TextureBufferDemo::Render()
{

}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureBufferDemo::MakeComputeShaderTexture()
{
	auto shader = make_shared<Shader>(L"26. TextureBufferDemo.fx");

	auto texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
	shared_ptr<TextureBuffer> textureBuffer = make_shared<TextureBuffer>(texture->GetTexture2D());

	shader->GetSRV("Input")->SetResource(textureBuffer->GetSRV().Get());
	shader->GetUAV("Output")->SetUnorderedAccessView(textureBuffer->GetUAV().Get());

	uint32 width = textureBuffer->GetWidth();
	uint32 height = textureBuffer->GetHeight();
	uint32 arraySize = textureBuffer->GetArraySize();

	uint32 x = max(1, (width + 31) / 32); //
	uint32 y = max(1, (height + 31) / 32); //
	shader->Dispatch(0, 0, x, y, arraySize);

	return textureBuffer->GetOutputSRV();
}

