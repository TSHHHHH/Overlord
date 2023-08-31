#pragma once

#include "Overlord/core.h"

namespace Overlord
{
	struct FramebufferSpec
	{
		uint32_t Width, Height;

		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		static Ref<Framebuffer> Create(const FramebufferSpec& spec);

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual const FramebufferSpec& GetSpec() const = 0;
	};
}