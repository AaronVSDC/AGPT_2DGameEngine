#include "SceneManager.h"

#include "Scene.h"

namespace Papyrus
{
	void SceneManager::update(float deltaTime)
	{
		for (auto& scene : m_pScenes)
		{
			if (scene->getIsActive()) scene->update(deltaTime);
		}
	}

	void SceneManager::fixedUpdate(float fixedTimeStep)
	{
		for (auto& scene : m_pScenes)
		{
			if (scene->getIsActive()) scene->fixedUpdate(fixedTimeStep);
		}
	}

	void SceneManager::render() const
	{
		for (const auto& scene : m_pScenes)
		{
			if (scene->getIsActive()) scene->render();
		}
	}

	void SceneManager::start()
	{
		for (auto& scene : m_pScenes)
		{
			if(scene->getIsActive()) scene->start();
		}
	}

	void SceneManager::onEnable()
	{
		for (auto& scene : m_pScenes)
		{
			if (scene->getIsActive()) scene->onEnable();
		}
	}

	void SceneManager::onDisable()
	{
		for (auto& scene : m_pScenes)
		{
			if(scene->getIsActive()) scene->onDisable();
		}
	}

	Scene* SceneManager::createScene(const std::string& name)
	{
		m_pScenes.push_back(std::unique_ptr<Scene>(new Scene(name)));
		return m_pScenes.back().get();
	}

	Scene* SceneManager::getCurrentScene() const
	{
		for (const auto& scene : m_pScenes)
		{
			if (scene->getIsActive()) return scene.get();
		}
		return nullptr;
	}
}