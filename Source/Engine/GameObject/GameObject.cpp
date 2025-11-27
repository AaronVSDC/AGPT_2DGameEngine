#include "GameObject.h"
#include "ResourceManager.h"


namespace Papyrus
{
	GameObject::~GameObject() 
	{
		for (auto& component : m_Components)
		{
			component->onDisable();
		}
	}
	void GameObject::update(float deltaTime)
	{
		for (auto& component : m_Components)
		{
			component->update(deltaTime);
		}
	}

	void GameObject::fixedUpdate(float fixedTimeStep)
	{
		for (auto& component : m_Components)
		{
			if (component->m_Enabled) component->fixedUpdate(fixedTimeStep);
		}
	}

	void GameObject::render() const
	{
		for (auto& component : m_Components)
		{
			if (component->m_Enabled) component->render();
		}
	}

	void GameObject::start()
	{
		for (auto& component : m_Components)
		{
			if (component->m_Enabled) component->start();
		}
	}

	void GameObject::onEnable()
	{
		for (auto& component : m_Components)
		{
			if (component->m_EnabledIsDirty)
			{
				component->onEnable();
				component->m_EnabledIsDirty = false;
			}
		}
	}

	void GameObject::onDisable()
	{
		for (auto& component : m_Components)
		{
			if (component->m_DisabledIsDirty)
			{
				component->onDisable();
				component->m_DisabledIsDirty = false;
			}
		}
	}

	void GameObject::setActive(bool active)
	{
		if (active and not m_IsActive)
		{
			for (auto& component : m_Components)
			{
				if (component->m_Enabled) component->m_EnabledIsDirty = true;
			}
		}
		if (not active and m_IsActive)
		{
			for (auto const& component : m_Components)
			{
				if (component->m_Enabled) component->m_DisabledIsDirty = true;
			}
		}
		m_IsActive = active;
	}
	  
	void GameObject::notifyTriggerEnter(GameObject* otherGameObject)
	{
		for (auto& component : m_Components)
			if (component->m_Enabled) component->onTriggerEnter(otherGameObject);
	}

	void GameObject::notifyTriggerExit(GameObject* otherGameObject)
	{
		for (auto& component : m_Components)
			if (component->m_Enabled) component->onTriggerExit(otherGameObject);
	}

	void GameObject::notifyCollisionEnter(GameObject* otherGameObject)
	{
		for (auto& component : m_Components)
			if (component->m_Enabled) component->onCollisionEnter(otherGameObject);
	}

	void GameObject::notifyCollisionExit(GameObject* otherGameObject)
	{
		for (auto& component : m_Components)
			if (component->m_Enabled) component->onCollisionExit(otherGameObject);
	}

	void GameObject::markForRemoval()
	{
		if (m_pendingRemoval) return; 
		m_pendingRemoval = true;
	}

}