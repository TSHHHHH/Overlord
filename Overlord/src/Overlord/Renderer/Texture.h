#pragma once

#include <string>

#include "Overlord/core.h"

namespace Overlord
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(const uint32_t slot = 0) const = 0;
	};

	class Textur2D : public Texture
	{
	public:
		static Ref<Textur2D> Create(const std::string& path);
	};
}