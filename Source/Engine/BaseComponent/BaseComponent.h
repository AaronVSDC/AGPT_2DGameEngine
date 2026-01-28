#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H
#include <cassert>
#include "PapyrusAPI.h"
namespace Papyrus
{
	class GameObject;
	class PAPYRUS_API BaseComponent 
	{
		friend class GameObject;
	public:

		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		virtual void update(float) {}
		virtual void fixedUpdate(float) {}
		virtual void render() const {}
		virtual void start() {}
		virtual void onEnable() {}
		virtual void onDisable() {} 

		virtual void onTriggerEnter(GameObject* otherGameObject) {}
		virtual void onTriggerExit(GameObject* otherGameObject) {}
		virtual void onCollisionEnter(GameObject* otherGameObject) {}
		virtual void onCollisionExit(GameObject* otherGameObject) {}
		virtual void onOwnerMarkedForRemoval() {}

		virtual GameObject* getOwner() const { return m_pOwner; }
		void setOwner(GameObject* owner)
		{
			assert(owner != nullptr and "BaseComponent::setOwner: owner is nullptr");
			if (!owner)
			{
				return;
			}
			m_pOwner = owner;
			//Note: SetOwner is called by GameObject::AddComponent<T> after the component is created
		}

		bool m_PendingRemove = false;
		bool m_Enabled = true;

	protected:
		//BaseComponent(GameObject& owner) { m_pOwner = &owner; }
		BaseComponent() = default;

	private:
		GameObject* m_pOwner = nullptr;
		bool m_EnabledIsDirty = true;
		bool m_DisabledIsDirty = false;


	};
}
#endif