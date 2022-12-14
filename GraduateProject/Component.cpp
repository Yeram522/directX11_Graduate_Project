#include "GameObject.h"
#include "Component.h"

ID3D11DeviceContext* Component::getDeviceContext()
{
	 return gameObject->m_deviceContext; 
}

Component::Component(GameObject* gameObject)
	:gameObject(gameObject),tag(gameObject->tag), transform(gameObject->transform)
{
	return;
}

void Component::updateHierachyInfo()
{
}

GameObject* Component::getParent() const { return gameObject->parent; }
Transform* Component::getTransform() const { return transform; }
