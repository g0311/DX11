#include "pch.h"
#include "Game.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "RenderManager.h"

unique_ptr<Game> GGame = make_unique<Game>();

Game::Game()
{
	
}

Game::~Game()
{
}

void Game::init(HWND hwnd)
{
	_hwnd = hwnd;

	//com°´Ã¼¶û ºñ½ÁÇÑµí
	_graphics = make_shared<Graphics>(hwnd);
	_pipeline = make_shared<Pipeline>(_graphics->GetDeviceContext());
	
	//Manager
	_input = make_shared<InputManager>();
	_input->Init(_hwnd);
	_time = make_shared<TimeManager>();
	_time->Init();
	_scene = make_shared<SceneManager>(_graphics);
	_scene->Init();
	_resource = make_shared<ResourceManager>(_graphics->GetDevice());
	_resource->Init();
	_render = make_shared<RenderManager>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	_render->Init();

	_scene->LoadScene(L"Test");
}

void Game::Update()
{
	_time->Update();
	_input->Update();
	_scene->Update();
}

void Game::Render()
{
	_render->Update(_graphics);
}