////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Transform.h"
#include "Model.h"


using namespace std;
////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class GameObject
{
private:
	string				tag;
	string				name;

	Transform* transform; // should not be nullptr

	friend class Component;

protected:

	GameObject* parent;
	vector<GameObject*> children;
	vector<Component*>	components;

	void add(GameObject* child) {
		if (!child) return;
		children.push_back(child);
	}
public:
	GameObject();
	GameObject(const GameObject&);
	~GameObject();

	Mesh* getMesh(){ return m_model->getMesh();}
	bool Initialize(D3DClass*, CameraClass*,const WCHAR*, const WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	void Draw(LightShaderClass* shader, LightClass* m_Light);
	
	template<typename T>
	void addComponent() {
		T* newComponent = new T(this);
		components.push_back(newComponent);
	}

	template<typename T>
	T* getComponent() {
		for (auto component : components) {
			auto result = dynamic_cast<T*>(component);
			if (result == nullptr) continue;
			return result;
		}
		return nullptr;
	}

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	Model* m_model;
};

#endif