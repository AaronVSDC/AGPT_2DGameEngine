#include "HealthUIComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "TextureComponent.h"
#include "SceneManager.h"

namespace xc
{

	void HealthUIComponent::render() const
	{
	}

	void HealthUIComponent::update(float dt)
	{
	}

	void HealthUIComponent::start()
	{

		m_HealthComponent = getOwner()->getComponent<HealthComponent>(); 


	}

}