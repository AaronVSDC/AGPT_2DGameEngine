#ifndef HEALTH_COMPONENT
#define HEALTH_COMPONENT

#include "BaseComponent.h"
#include <algorithm>

namespace xc
{
	class HealthComponent final : public Papyrus::BaseComponent
	{
	public:
		HealthComponent(int maxHealth = 3)
			: m_MaxHealth(maxHealth), m_Health(maxHealth) {
		}

		int  getHealth() const { return m_Health; }
		int  getMaxHealth() const { return m_MaxHealth; }
		bool isDead() const { return m_Health <= 0; }

		void damage(int amount)
		{
			m_Health = std::max(0, m_Health - amount);
		}

		void heal(int amount)
		{
			m_Health = std::min(m_MaxHealth, m_Health + amount);
		}

	private:
		int m_MaxHealth;
		int m_Health;
	};
}

#endif
