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

	GameObject(string name = "unknown", string tag = "unknown",
		D3DClass* m_D3D = nullptr, CameraClass* m_Camera = nullptr,  // transform
		GameObject* parent = nullptr);
	GameObject(const GameObject&);
	~GameObject();

	bool Initialize(D3DClass*, CameraClass*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	
	void setParent(GameObject* parent) {
		if (this->parent) {
			this->parent->remove(this);
		}
		/* TODO */
		// if previous parent is a root meaning parent is nullptr, 
		// we need extra treatment on the root node.

		this->parent = parent;
		if (parent) {
			parent->add(this);
		}
	}

	void remove(GameObject* child) {
		if (!child) return;
		auto it = find(children.begin(), children.end(), child);
		if (it == children.end()) return;
		children.erase(it);
	}

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

	template<typename T>
	T* getOrAddComponent() {
		auto comp = getComponent<T>();
		if (comp) return comp;
		addComponent<T>();
		return getComponent<T>();
	}

	void updateHierachy();

	vector<GameObject*> getchildrens()  { return children; }
	string getname();
private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	CameraClass* m_Camera;

};

#endif