#include "SpeedPowerUp.h"


SpeedPowerUp::SpeedPowerUp(TexturedModel texuredModel, Transform transform)
{
	gameObject = new GameObject(texuredModel, transform);
	gameObject->name = "powerUp";
	vec3 offset = vec3(0.3f, 0.3f, 0.3f);
	boxCollider = BoxCollider(gameObject->transform.position - offset,
		gameObject->transform.position + offset,
		this->gameObject->name);
	enabled = true;
}

HitInfo SpeedPowerUp::IsColliding(BoxCollider other)
{
	return boxCollider.OnCollision(other);
}

BoxCollider SpeedPowerUp::GetBoxCollider()
{
	return boxCollider;
}

void SpeedPowerUp::UpdateCollider()

{
	vec3 offset = vec3(0.3f, 0.3f, 0.3f);
	this->boxCollider.SetMinExtent(gameObject->transform.position - offset);
	this->boxCollider.SetMaxExtent(gameObject->transform.position + offset);
}

bool SpeedPowerUp::IsEnabled()
{
	return enabled;
}

void SpeedPowerUp::Disable()
{
	enabled = false;
}

void SpeedPowerUp::Enable()
{
	enabled = true;
}
