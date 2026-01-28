#ifndef HEALTH_UI_COMPONENT
#define HEALTH_UI_COMPONENT

#include "BaseComponent.h"
#include <memory>
#include <string>

namespace Papyrus { class Scene; class GameObject; }

namespace xc
{
	class HealthComponent; 
	class HealthUIComponent final : public Papyrus::BaseComponent
	{
	public:
		
		void render() const override; 
		void update(float dt) override;
		void start() override; 
	private:
		std::unique_ptr<Papyrus::GameObject> m_Life1;
		std::unique_ptr<Papyrus::GameObject> m_Life2;
		HealthComponent* m_HealthComponent; 


	};
}

#endif
