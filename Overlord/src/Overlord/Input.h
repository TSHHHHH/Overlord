#pragma once

#include "Overlord/core.h"

#include "KeyCodes.h"
#include "MouseButtonCodes.h"

namespace Overlord
{
	class Input
	{
	public:
		static bool IsKeyPressed(int key);

		static bool IMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

		//inline static bool IsKeyPressed(int keycode)
		//{
		//	return s_Instance->IsKeyPressedImpl(keycode);
		//}

		//inline static bool IsMouseButtonPressed(int mouseButton)
		//{
		//	return s_Instance->IsMouseButtonPressedImpl(mouseButton);
		//}

		//inline static std::pair<float, float> GetMousePosition()
		//{
		//	return s_Instance->GetMousePostionImpl();
		//}

		//inline static float GetMouseX()
		//{
		//	return s_Instance->GetMouseXImpl();
		//}

		//inline static float GetMouseY()
		//{
		//	return s_Instance->GetMouseYImpl();
		//}

		//protected:
		//	virtual bool IsKeyPressedImpl(int keycode) = 0;

		//	virtual bool IsMouseButtonPressedImpl(int mouseButton) = 0;

		//	virtual std::pair<float, float> GetMousePostionImpl() = 0;
		//	virtual float GetMouseXImpl() = 0;
		//	virtual float GetMouseYImpl() = 0;

		//private:
		//	static Input* s_Instance;
	};
}