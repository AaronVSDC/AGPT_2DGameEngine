#include "Scene.h"
#include "GameObject.h"

#include <algorithm>
#include <memory>
namespace Papyrus
{
	unsigned int Scene::m_idCounter = 0;

	Scene::Scene(const std::string& name) : m_Name(name) {}

	Scene::~Scene() = default;

	void Scene::add(std::unique_ptr<GameObject> object)
	{
		// NEVER push into m_Objects directly during iteration
		m_PendingAdds.emplace_back(std::move(object));
	}
	void Scene::flushPendingAdds()
	{
		if (m_PendingAdds.empty())
			return;

		for (auto& object : m_PendingAdds)
		{
			// If the scene already started, newly spawned objects must be started/enabled now
			if (m_HasStarted && object->getIsActive())
			{
				object->start();
				object->onEnable();
			}

			m_Objects.emplace_back(std::move(object));
		}

		m_PendingAdds.clear();
	}
	void Scene::removeAll()
	{
		m_Objects.clear();
	}

	void Scene::update(float deltaTime)
	{
		for (auto& object : m_Objects)
		{
			if (object->getIsActive())
				object->update(deltaTime);
		}

		flushPendingAdds();        
		checkRemovalGameObject();  
	}


	void Scene::fixedUpdate(float fixedTimeStep)
	{
		for (auto& object : m_Objects)
		{
			if (object->getIsActive())
				object->fixedUpdate(fixedTimeStep);
		}

		flushPendingAdds(); 
	}


	void Scene::render() const
	{
		for (const auto& object : m_Objects)
		{
			if(object->getIsActive()) object->render(); 
		}
	}

	void Scene::start()
	{
		for (auto& object : m_Objects)
		{
			if (object->getIsActive())
				object->start();
		}

		m_HasStarted = true;
		flushPendingAdds(); 
	}

	void Scene::onEnable()
	{
		for (auto& object : m_Objects)
		{
			object->onEnable();
		}
	}

	void Scene::onDisable()
	{
		for (auto& object : m_Objects)
		{
			object->onDisable();
		}
	}

	void Scene::checkRemovalGameObject()
	{
		std::erase_if(m_Objects, [](const std::unique_ptr<GameObject>& obj)
			{
				return obj->isPendingRemoval();
			});

	}

	GameObject* Scene::findGameObjectByTag(std::string_view tag) noexcept
	{
		for (auto& obj : m_Objects)
		{
			if (!obj) continue;
			if (obj->getTag() == tag)
				return obj.get();
		}
		return nullptr;
	}
	std::vector<GameObject*> Scene::findGameObjectsByTag(std::string_view tag) noexcept
	{
		std::vector<GameObject*> out;
		out.reserve(4);

		for (auto& obj : m_Objects)
		{
			if (!obj) continue;
			if (obj->getTag() == tag)
				out.push_back(obj.get());
		}
		return out;
	}
}