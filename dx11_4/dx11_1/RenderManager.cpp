#include "pch.h"
#include "RenderManager.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Game.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "Animator.h"

RenderManager::RenderManager(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: _device(device), _deviceContext(deviceContext)
{
}

RenderManager::~RenderManager()
{
}

void RenderManager::Init()
{
	_pipeline = make_shared<Pipeline>(_deviceContext);

	_cameraBuffer = make_shared<ConstantBuffer<CameraData>>(_device, _deviceContext);
	_cameraBuffer->Create();
	_transformBuffer = make_shared<ConstantBuffer<TransformData>>(_device, _deviceContext);
	_transformBuffer->Create();
	_animationBuffer = make_shared<ConstantBuffer<AnimationData>>(_device, _deviceContext);
	_animationBuffer->Create();

	_rasterizerState = make_shared<RasterizerState>(_device);
	_rasterizerState->Create();
	_blendState = make_shared<BlendState>(_device);
	_blendState->Create();
	_samplerState = make_shared<SamplerState>(_device);
	_samplerState->Create();
}

void RenderManager::Update(shared_ptr<Graphics> graphics)
{
	graphics->RenderBegin();
	
	//render
	PushCameraData();
	GatherRenderableObjects();
	RenderObjects();

	graphics->RenderEnd();
}

void RenderManager::PushCameraData()
{
	_cameraData.matView = Camera::S_MatView;
	//_cameraData.matView = Matrix::Identity;
	_cameraData.matProjection = Camera::S_MatProjection;
	//_cameraData.matProjection = Matrix::Identity;
	_cameraBuffer->CopyData(_cameraData);
}

void RenderManager::PushTransformData()
{
	_transformBuffer->CopyData(_transformData);
}

void RenderManager::PushAnimationData()
{
	_animationBuffer->CopyData(_animationData);
}

void RenderManager::GatherRenderableObjects()
{
	_renderObjects.clear();

	auto& gameObjects = SCENE->GetActiveScene()->GetGameObjects();
	for (const shared_ptr<GameObject>& gameObject : gameObjects)
	{
		shared_ptr<MeshRenderer> meshRenderer = gameObject->GetMeshRenderer();
		if (meshRenderer)
		{
			_renderObjects.push_back(gameObject);
		}
	}
}

void RenderManager::RenderObjects()
{
	for (const shared_ptr<GameObject>& gameObject : _renderObjects)
	{
		shared_ptr<MeshRenderer> meshRenderer = gameObject->GetMeshRenderer();
		if (meshRenderer == nullptr)
			continue;
		shared_ptr<Transform> transform = gameObject->GetTransform();
		if (transform == nullptr)
			continue;
		//SRT
		_transformData.matWorld = transform->GetWorldMatrix();
		PushTransformData();
		
		//Animation
		shared_ptr<Animator> animator = gameObject->GetAnimator();
		if (animator)
		{
			const Keyframe& keyframe = animator->GetCurrnetKeyFrame();
			_animationData.spriteOffset = keyframe.offset;
			_animationData.spriteSize = keyframe.size;
			_animationData.textureSize = animator->GetCurrentAnimation()->GetTextureSize();
			_animationData.useAnimation = 1.f;
			PushAnimationData();
			_pipeline->SetConstantBuffer(2, SS_VertexShader, _animationBuffer);
			_pipeline->SetTexture(0, SS_PixelShader, animator->GetCurrentAnimation()->GetTexture());
		}
		else
		{
			_animationData.spriteOffset = Vec2{ 0.f, 0.f };
			_animationData.spriteSize = Vec2{ 0.f, 0.f };
			_animationData.textureSize = Vec2{ 0.f, 0.f };
			_animationData.useAnimation = 0.f;
			PushAnimationData();
			_pipeline->SetConstantBuffer(2, SS_VertexShader, _animationBuffer);
			_pipeline->SetTexture(0, SS_PixelShader, meshRenderer->GetTexture());
		}
		PipelineInfo info;
		info.inputLayout = meshRenderer->GetShader()->GetInputLayout();
		info.vertexShader = meshRenderer->GetShader()->GetVertexShader();
		info.pixelShader = meshRenderer->GetShader()->GetPixelShader();
		info.rasterizerState = _rasterizerState;
		info.blendState = _blendState;
		_pipeline->UpdatePipeline(info);

		_pipeline->SetVertexBuffer(meshRenderer->GetMesh()->GetVertexBuffer());
		_pipeline->SetIndexBuffer(meshRenderer->GetMesh()->GetIndexBuffer());
		_pipeline->SetConstantBuffer(0, SS_VertexShader, _cameraBuffer);
		_pipeline->SetConstantBuffer(1, SS_VertexShader, _transformBuffer);
		//_pipeline->SetTexture(0, SS_PixelShader, meshRenderer->GetTexture());
		_pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);

		_pipeline->DrawIndexed(meshRenderer->GetMesh()->GetIndexBuffer()->GetCount(), 0, 0);
	}
}//무조건 파이프라인 순서를 지켜야 할 필요는 없다..? 컴퓨터가 알아서 수행? >> dx가 명령어 스케쥴링 해준대
