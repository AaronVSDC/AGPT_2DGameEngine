#include "DecideCollisionComponent.h"
#include "GameObject.h"
#include "ExplosionUtility.h"
#include "HealthComponent.h"

namespace xc
{
	void DecideCollisionComponent::onTriggerEnter(Papyrus::GameObject* other)
	{
		if (!other) return;

		auto* health = getOwner()->getComponent<HealthComponent>();
		if (!health || health->isDead()) return;

		if (other->getTag() == "Aestroid" ||
			other->getTag() == "Enemy" ||
			other->getTag() == "Indestructible")
		{
			static Papyrus::GameObject* lastHit = nullptr;
			if (lastHit == other) return;
			lastHit = other;

			health->damage(1);

			if (health->isDead())
			{
				explodeAndDie(
					getOwner(),
					"Resources/Textures/explode64.bmp",
					5, 2, 10, 16.0f
				);
			}
		}
		else if (other->getTag() == "PowerUp")
		{
			if (health)
				health->heal(1);

			other->markForRemoval();
		}
	}
}
