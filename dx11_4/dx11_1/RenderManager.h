#pragma once
#include "RenderHelper.h"

class RenderManager
{
public:
	RenderManager(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~RenderManager();

	void Init(); //버퍼 생성
	void Update(shared_ptr<Graphics> graphics); //렌더 비긴, 렌더 엔드 수행 위해 그래픽 받음

private:
	void PushCameraData(); //카메라 데이터 버퍼 전송
	void PushTransformData(); //트랜스폼 데이터 버퍼 전송
	void PushAnimationData(); //애니메이션 데이터 버퍼 전송

	void GatherRenderableObjects(); //모든 게임 오브젝트 가져오기
	void RenderObjects(); //모든 오브젝트에 대해서 렌더 실시

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
	
	shared_ptr<Pipeline> _pipeline; //파이프라인 직접 만들어서 렌더링 수행 할 것

private:
	//Camera
	CameraData _cameraData;
	shared_ptr<ConstantBuffer<CameraData>> _cameraBuffer;
	//SRT
	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _transformBuffer;
	//Animation
	AnimationData _animationData;
	shared_ptr<ConstantBuffer<AnimationData>> _animationBuffer;

private:
	shared_ptr<RasterizerState> _rasterizerState;
	shared_ptr<SamplerState> _samplerState;
	shared_ptr<BlendState> _blendState;

private:
	vector<shared_ptr<GameObject>> _renderObjects;
};

