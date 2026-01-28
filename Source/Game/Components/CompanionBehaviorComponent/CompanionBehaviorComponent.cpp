#include "CompanionBehaviorComponent.h"
#include "SceneManager.h"
 

namespace xc
{
	CompanionBehaviorComponent::CompanionBehaviorComponent(glm::vec2 offset)
		:m_Offset{ offset }
	{
	}
	void CompanionBehaviorComponent::update(float deltaTime)
	{
		auto& position = getOwner()->m_Transform.position;
		position = m_pPlayer->m_Transform.position;

		position.x += m_Offset.x;
		position.y += m_Offset.y;
	}

	void CompanionBehaviorComponent::start()
	{
		m_pPlayer = Papyrus::SceneManager::getInstance().getCurrentScene()->findGameObjectByTag("Player"); 
	}

	void CompanionBehaviorComponent::onTriggerEnter(Papyrus::GameObject* other)
	{

	}

}