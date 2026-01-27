#ifndef GAME_OBJECT_H 
#define GAME_OBJECT_H
#include <memory>
#include <string>
#include <vector>
#include "BaseComponent.h"
#include "Transform.h"
#include "PapyrusAPI.h"

namespace Papyrus
{
	class Texture2D;

	class PAPYRUS_API GameObject final   
	{
	public:
		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void update(float deltaTime);
		void fixedUpdate(float fixedTimeStep);
		void render() const;

		void start();
		void onEnable();
		void onDisable();
		void setActive(bool active);

		void notifyTriggerEnter(GameObject* otherGameObject);
		void notifyTriggerExit(GameObject* otherGameObject); 
		void notifyCollisionEnter(GameObject* otherGameObject);
		void notifyCollisionExit(GameObject* otherGameObject);

		Transform getTransform() const { return m_Transform; }
		bool getIsActive() const { return m_IsActive; }

		//component stuff
		void addComponent(std::unique_ptr<BaseComponent> component)
		{
			component->setOwner(this);
			m_Components.emplace_back(std::move(component));
		}
		template <typename T> void removeComponent()
		{

			if (m_Components.empty())
				return;

			for (auto& component : m_Components)
			{
				if (auto* castedComponent = dynamic_cast<T*>(component.get()))
				{
					castedComponent->m_PendingRemove = true;
				}
			}
		}

		template <typename T> T* getComponent()
		{

			if (m_Components.empty())
				return nullptr;

			for (const auto& component : m_Components)
			{
				if (auto* castedComponent = dynamic_cast<T*>(component.get()))
				{
					return castedComponent;
				}
			}

			return nullptr;
		}
		template <typename T> std::vector<T*> getComponents()
		{
			std::vector<T*> components;
			for (auto& component : m_Components) {
				if (auto casted = dynamic_cast<T*>(component.get())) {
					components.push_back(casted);
				}
			}
			return components;
		}
		template <typename T> bool hasComponent() const
		{
			for (const auto& component : m_Components)
			{
				if (auto* castedComponent = dynamic_cast<T*>(component.get()))
				{
					return true;
				}
			}

			return false;
		}
		 
		void markForRemoval();
		bool isPendingRemoval() const noexcept { return m_pendingRemoval; }

		void setTag(const std::string& tag) { m_Tag = tag; } 
		const std::string& getTag() const { return m_Tag; } 

		Transform m_Transform = {};

	private:
		bool m_pendingRemoval = false;

		std::vector<std::unique_ptr<BaseComponent>> m_Components = {};

		bool m_IsActive = true;

		std::string m_Tag = {};

	};
}
#endif