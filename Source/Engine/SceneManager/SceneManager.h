#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <vector> 
#include <string>
#include <memory>
#include "Singleton.h"
#include "Scene.h"
#include "PapyrusAPI.h"

namespace Papyrus 
{
	class PAPYRUS_API SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene* createScene(const std::string& name);
		Scene* getCurrentScene() const;

		void update(float deltaTime);
		void fixedUpdate(float fixedTimeStep);
		void render() const;
		void start();
		void onEnable();
		void onDisable();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_pScenes;
	};
}
#endif