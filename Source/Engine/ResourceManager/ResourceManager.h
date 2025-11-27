#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <filesystem>
#include <string>
#include <memory> 
#include <map>
#include "Singleton.h"
#include "Texture2D.h"
namespace Papyrus
{
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void init(const std::filesystem::path& data);
		Texture2D* loadTexture(const std::string& file);
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::filesystem::path m_dataPath;

		std::map<std::string, std::unique_ptr<Texture2D>> m_pLoadedTextures;

	};
}
#endif