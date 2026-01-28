#include "DecideCollisionComponent.h"
#include "GameObject.h"
#include "ExplosionUtility.h"
namespace xc
{
	void DecideCollisionComponent::onTriggerEnter(Papyrus::GameObject* other)
	{
		if (!other) return; 

		if (other->getTag() == "Aestroid")
		{
			explodeAndDie(
				getOwner(),
				"Resources/Textures/explode64.bmp",
				5, 2, 10, 16.0f
			);
			return; 
		}
		if (other->getTag() == "Enemy")
		{
			explodeAndDie(
				getOwner(),
				"Resources/Textures/explode64.bmp",
				5, 2, 10, 16.0f
			);
		}
		else if (other->getTag() == "Indestructible") 
		{
			explodeAndDie( 
				getOwner(), 
				"Resources/Textures/explode64.bmp",
				5, 2, 10, 16.0f
			);
		}

		else if (other->getTag() == "PowerUp")  
		{
			other->markForRemoval(); 
		}

	}

}