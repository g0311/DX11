#include "pch.h"
#include "InputManager.h"

void InputManager::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

void InputManager::Update()
{
	// 현재 활성화된 윈도우를 가져옵니다.
	HWND hwnd = ::GetActiveWindow();

	// 현재 활성화된 윈도우가 이전에 저장한 윈도우와 다르다면,
	// 모든 키의 상태를 NONE으로 설정하고 메서드를 종료합니다.
	if (_hwnd != hwnd)
	{
		for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
			_states[key] = KEY_STATE::NONE;

		return;
	}

	// 키보드의 상태를 저장할 배열을 선언하고 초기화합니다.
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};

	// 키보드의 상태를 가져옵니다. 실패하면 메서드를 종료합니다.
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	// 각 키의 상태를 업데이트합니다.
	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// 키가 눌려 있으면 true
		if (asciiKeys[key] & 0x80)
		{
			KEY_STATE& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			else
				state = KEY_STATE::DOWN;
		}
		else
		{
			KEY_STATE& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::UP;
			else
				state = KEY_STATE::NONE;
		}
	}

	// 마우스 커서의 위치를 가져옵니다.
	::GetCursorPos(&_mousePos);

	// 스크린 좌표를 클라이언트 좌표로 변환합니다.
	::ScreenToClient(_hwnd, &_mousePos);
}
