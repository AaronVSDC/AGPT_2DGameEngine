#include "HealthUIComponent.h"
#include "HealthComponent.h"
#include "GameObject.h"

namespace xc
{
	void HealthUIComponent::update(float)
	{
		if (!m_HealthComponent)
			m_HealthComponent = getOwner()->getComponent<HealthComponent>();

		if (!m_HealthComponent)
			return;

		const int hp = m_HealthComponent->getHealth();

		for (int i = 0; i < (int)m_Lives.size(); ++i)
		{
			if (m_Lives[i])
				m_Lives[i]->setActive(i < hp);
		}
	}
}
