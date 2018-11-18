#include "Texture.h"

#include <glad/glad.h>
#include "stb_image.h"

void CTexture::CreateTexture2D(const std::string& textureName, bool flip /*= true*/)
{
	DeleteTexture();

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	stbi_set_flip_vertically_on_load(static_cast<unsigned int>(flip));

	int width, height, nrComponents;
	unsigned char *data = stbi_load((m_mainDirectory + '/' + textureName).c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}
	else
	{
		printf_s("Failed to load texture: %s\n", textureName.c_str());
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CTexture::CreateCubemap(std::vector<std::string>& faces, bool texFlip/*= true*/)
{
	DeleteTexture();

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			printf_s("Failed to load cubemap, fail at texture: %s\n", faces[i].c_str());
		}

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}


void CTexture::Use(unsigned int textureUnit/* = 0*/)
{
    glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void CTexture::Disable()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CTexture::DeleteTexture()
{
	glDeleteTextures(1, &m_textureID);
}
