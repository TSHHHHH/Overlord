#pragma once

#include <memory>
#include "Overlord/Renderer/Buffer.h"

namespace Overlord
{
	class VertexArray
	{
	protected:
		using VertexBufferList = std::vector<Ref<VertexBuffer>>;

	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const VertexBufferList& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}