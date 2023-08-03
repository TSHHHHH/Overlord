#pragma once

#include "Overlord/Renderer/VertexArray.h"

namespace Overlord
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(Ref<IndexBuffer>& indexBuffer) override;

		virtual const VertexBufferList& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

	private:
		uint32_t m_ArrayID;

		VertexBufferList m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}