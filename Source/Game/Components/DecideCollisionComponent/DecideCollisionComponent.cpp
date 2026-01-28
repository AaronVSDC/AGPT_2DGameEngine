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

		else if (other->getTag() == "ShieldPowerUp")  
		{
			other->markForRemoval(); 
		}
		else if (other->getTag() == "WeaponPowerup")
		{

		}


	}

}