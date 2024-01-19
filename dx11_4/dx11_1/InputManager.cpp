#include "pch.h"
#include "InputManager.h"

void InputManager::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

void InputManager::Update()
{
	// ���� Ȱ��ȭ�� �����츦 �����ɴϴ�.
	HWND hwnd = ::GetActiveWindow();

	// ���� Ȱ��ȭ�� �����찡 ������ ������ ������� �ٸ��ٸ�,
	// ��� Ű�� ���¸� NONE���� �����ϰ� �޼��带 �����մϴ�.
	if (_hwnd != hwnd)
	{
		for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
			_states[key] = KEY_STATE::NONE;

		return;
	}

	// Ű������ ���¸� ������ �迭�� �����ϰ� �ʱ�ȭ�մϴ�.
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};

	// Ű������ ���¸� �����ɴϴ�. �����ϸ� �޼��带 �����մϴ�.
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	// �� Ű�� ���¸� ������Ʈ�մϴ�.
	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// Ű�� ���� ������ true
		if (asciiKeys[key] & 0x80)
		{
			KEY_STATE& state = _states[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			else
				state = KEY_STATE::DOWN;
		}
		else
		{
			KEY_STATE& state = _states[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::UP;
			else
				state = KEY_STATE::NONE;
		}
	}

	// ���콺 Ŀ���� ��ġ�� �����ɴϴ�.
	::GetCursorPos(&_mousePos);

	// ��ũ�� ��ǥ�� Ŭ���̾�Ʈ ��ǥ�� ��ȯ�մϴ�.
	::ScreenToClient(_hwnd, &_mousePos);
}
