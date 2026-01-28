#ifndef COMPANION_COMPONENT
#define COMPANION_COMPONENT

#include "BaseComponent.h"

#include "glm/glm.hpp"


namespace xc 
{
	class CompanionBehaviorComponent final : public Papyrus::BaseComponent
	{
	public: 
		CompanionBehaviorComponent(glm::vec2 offsetWhenPickedup);

		void update(float deltaTime) override; 
		void start() override;

		void onTriggerEnter(Papyrus::GameObject* other) override; 

	private: 

		Papyrus::GameObject* m_pPlayer; 
		glm::vec2 m_Offset; 



	};





}






#endif