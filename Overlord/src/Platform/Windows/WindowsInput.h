#pragma once

#include "Overlord/Input.h"

namespace Overlord
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int mouseButton) override;
		virtual std::pair<float, float> GetMousePostionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}