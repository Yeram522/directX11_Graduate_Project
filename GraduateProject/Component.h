////////////////////////////////////////////////////////////////////////////////
// Filename: Component.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _COMPONENT_H_
#define _COMPONENT_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include <string>
#include <vector>
#include <directxmath.h>
#include <d3d11.h>

using namespace DirectX;
using namespace std;

class GameObject;
class Transform;

class Component
{
	string tag;
protected:
	GameObject* getParent() const;
	Transform* getTransform() const;
	void setTag(string tag)
	{
		this->tag = tag;
	}
	GameObject* gameObject;
	Transform* transform;

	bool enabled;

	ID3D11DeviceContext* getDeviceContext();
public:
	Component(GameObject* gameObject);

	virtual ~Component() {}

	friend class GameObject;

	virtual void update() {}

	void setEnable(bool enable = true) { this->enabled = enable; }
	bool getEnable() const { return enabled; }

};

#endif

