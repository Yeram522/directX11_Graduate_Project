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
	void SetBillBoardTransform();
	void FollowCamera();

	XMMATRIX m_worldMatrix, m_viewMatrix, m_projectionMatrix;

	bool m_BillBoard= false;
	D3DClass* m_D3D;//임시로 public
	CameraClass* m_Camera;//임시로 public
private:
	XMFLOAT3 localposition = { 0,0,0 };
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

	XMFLOAT3 parentposition;



};

#endif

