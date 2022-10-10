#include "GameObject.h"
#include "Component.h"

Component::Component(GameObject* gameObject)
	:gameObject(gameObject),tag(gameObject->tag), transform(gameObject->transform)
{
}

GameObject* Component::getParent() const { return gameObject->parent; }
Transform* Component::getTransform() const { return transform; }
