////////////////////////////////////////////////////////////////////////////////
// Filename: Transform.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _Transform_H_
#define _Transform_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "cameraclass.h"
#include "d3dclass.h"
#include "Component.h"

class Transform : public Component
{
public:
	Transform(GameObject* gameObject);
	bool Initialize(D3DClass*, CameraClass*);
	void update() override;
	void Rotate();
	void SetPosition(float, float, float);
	XMMATRIX m_worldMatrix, m_viewMatrix, m_projectionMatrix;

	D3DClass* m_D3D;//�ӽ÷� public
	CameraClass* m_Camera;//�ӽ÷� public
private:
	XMFLOAT3 localposition;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

	XMFLOAT3 parentposition;



};

#endif

