#ifndef DECIDE_COLLISION_COMPONENT_H
#define DECICE_COLLISION_COMPONENT_H

#include "BaseComponent.h"
namespace xc
{
	class DecideCollisionComponent final : public Papyrus::BaseComponent
	{
	public: 

		void onTriggerEnter(Papyrus::GameObject* other) override; 



	private:
	};





}



#endif