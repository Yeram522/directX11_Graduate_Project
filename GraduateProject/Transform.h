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
	void SetScale(float, float, float);
	void SetBillBoardTransform();
	void FollowCamera();
	void FollowUpCamera();
	XMMATRIX m_worldMatrix, m_viewMatrix, m_projectionMatrix;

	bool m_BillBoard= false;
	D3DClass* m_D3D;//임시로 public
	CameraClass* m_Camera;//임시로 public


	//for imgui
	float* getLocalPositionToPfloat()
	{
		return m_flocalposition;
	}
private:
	XMFLOAT3 localposition = { 0,0,0 };
	XMFLOAT3 rotation;
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };

	XMFLOAT3 parentposition;
	float* m_flocalposition;


};

#endif

