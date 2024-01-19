#pragma once

#include "Types.h"
#include "Values.h"
#include "Struct.h"

//STL
#include <memory>
#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <unordered_map>
using namespace std;

//WIN
#include <windows.h>
#include <assert.h>

//DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
//c++ �߰� ���� ���͸� ������ ���༭ ã�� �� ����
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif // _DEBUG

#define CHECK(p) assert(SUCCEEDED(p))

#define GAME  GGame

#define SCENE GAME->GetSceneManager()
#define TIME GAME->GetTimeManager()
#define INPUT GAME->GetInputManager()
#define RESOURCES GAME->GetResourceManager()
#define RENDER GAME->GetRenderManager()

//Engine
#include "Graphics.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "Geometry.h"
#include "GeometryHelper.h"
#include "VertexData.h"
#include "ShaderBase.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "BlendState.h"
#include "Pipeline.h"
#include "Transform.h"