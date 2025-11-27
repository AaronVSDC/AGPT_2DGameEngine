#include <stdexcept>
#include "ResourceManager.h"
#include "Renderer.h"


namespace fs = std::filesystem;
namespace Papyrus
{
	void ResourceManager::init(const std::filesystem::path& dataPath)
	{
		m_dataPath = dataPath;
	}

	Texture2D* ResourceManager::loadTexture(const std::string& file)
	{
		const auto fullPath = m_dataPath / file;
		const auto filename = fs::path(fullPath).filename().string();
		if (m_pLoadedTextures.find(filename) == m_pLoadedTextures.end())
			m_pLoadedTextures.insert(std::pair(filename, std::make_unique<Texture2D>(fullPath.string())));

		return m_pLoadedTextures.at(filename).get();
	}
}