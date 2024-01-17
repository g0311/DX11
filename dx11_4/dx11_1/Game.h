#pragma once
#include "Graphics.h"
#include "GameObject.h"

class Game
{
public:
	Game();
	~Game();

public:
	void init(HWND hwnd);
	void Update();
	void Render();

private:
	HWND _hwnd;

	shared_ptr<Graphics> _graphics;
	shared_ptr<Pipeline> _pipeline;

	//Temp
	shared_ptr<GameObject> _monster;
	shared_ptr<GameObject> _camera;
};

