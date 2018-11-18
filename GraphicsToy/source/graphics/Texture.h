#pragma once

#include <string>
#include <vector>

class CTexture
{
public:
	CTexture() = default;

	CTexture(const std::string& mainTextureDirectory)
		: m_mainDirectory(mainTextureDirectory)
	{
	}

	void CreateTexture2D(const std::string& textureName, bool flip = true);
	void CreateCubemap(std::vector<std::string>& faces, bool texFlip = true);

	void Use(unsigned int textureUnit = 0);
	void Disable();

	void DeleteTexture();

    operator unsigned int() { return m_textureID; };

	std::string GetTexturesDir() const				{ return m_mainDirectory; };
	void SetTexturesDir(const std::string& newDir)	{ m_mainDirectory = newDir; };

private:
	std::string		m_mainDirectory;
	unsigned int	m_textureID;
};