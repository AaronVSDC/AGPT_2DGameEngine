#ifndef HEALTH_COMPONENT
#define HEALTH_COMPONENT

#include "BaseComponent.h"

namespace xc
{
	class HealthComponent final : public Papyrus::BaseComponent
	{
	public:

		int  getHealth() const { return m_Health; }
		bool isDead() const { return m_Health <= 0; }

		void damage(int amount)
		{
			m_Health -= amount;
		}

	private:
		int m_Health = 2;
	};
}

#endif
