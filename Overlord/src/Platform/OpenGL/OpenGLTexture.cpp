#include "oldpch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>

#include "stb_image.h"

namespace Overlord
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;

		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		OLD_CORE_ASSERT(data, "Failed to load image!!");

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		switch (channels)
		{
		case 3:
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
			break;

		case 4:
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
			break;
		}

		OLD_CORE_ASSERT(internalFormat & dataFormat, "Image Color Format not supported!!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(const uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}