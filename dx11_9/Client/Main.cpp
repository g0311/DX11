#include "pch.h"
#include "Main.h"
#include "Engine/Game.h" //엔진의 게임 클래스
#include "19. InstancingDemo.h"
#include "20. MeshInstancingDemo.h"
#include "21. ModelInstancingDemo.h"
#include "22. AnimInstancingDemo.h"
#include "23. RenderDemo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	GameDesc desc;
	desc.appName = L"GameCoding";
	desc.hInstance = hInstance;
	desc.vsync = false;
	desc.hWnd = NULL;
	desc.width = 800;
	desc.height = 600;
	desc.clearColor = Color(0.f, 0.f, 0.f, 0.f);
	desc.app = make_shared<RenderDemo>();

	GAME->Run(desc);

	return 0;
}