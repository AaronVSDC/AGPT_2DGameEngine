#ifndef HEALTH_UI_COMPONENT
#define HEALTH_UI_COMPONENT

#include "BaseComponent.h"
#include <vector>

namespace Papyrus { class GameObject; }

namespace xc
{
	class HealthComponent;

	class HealthUIComponent final : public Papyrus::BaseComponent
	{
	public:
		void setLifeIcons(const std::vector<Papyrus::GameObject*>& lives)
		{
			m_Lives = lives;
		}

		void update(float dt) override;

	private:
		HealthComponent* m_HealthComponent = nullptr;
		std::vector<Papyrus::GameObject*> m_Lives;
	};
}

#endif
