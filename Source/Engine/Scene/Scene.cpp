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
		m_Objects.emplace_back(std::move(object));
	}

	void Scene::removeAll()
	{
		m_Objects.clear();
	}

	void Scene::update(float deltaTime)
	{
		for (auto& object : m_Objects)
		{
			if (object->getIsActive()) object->update(deltaTime);
		}
		checkRemovalGameObject();   
	}

	void Scene::fixedUpdate(float fixedTimeStep)
	{
		for (auto& object : m_Objects)
		{
			if (object->getIsActive()) object->fixedUpdate(fixedTimeStep);
		}
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
			if (object->getIsActive()) object->start();
		}
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
}