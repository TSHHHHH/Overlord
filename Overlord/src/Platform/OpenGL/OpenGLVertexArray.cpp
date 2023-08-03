#include "oldpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Overlord
{
	// For glVertexAttribPointer 3rd argument
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Bool:		return GL_BOOL;
		}

		OLD_CORE_ASSERT(false, "Unknow Shader Data Type!!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_ArrayID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_ArrayID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_ArrayID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer>& vertexBuffer)
	{
		OLD_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), 
			"Error! Trying to bind a vertex buffer without a layout!!");

		glBindVertexArray(m_ArrayID);
		vertexBuffer->Bind();

		// Set vertex attributes with for range loop
		const auto& layout = vertexBuffer->GetLayout();
		uint32_t index = 0;
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.isNormalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);

			++index;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_ArrayID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}