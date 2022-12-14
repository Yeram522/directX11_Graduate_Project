////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <directxmath.h>

#include "AlignedAllocationPolicy.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass : public AlignedAllocationPolicy<16>
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void RenderReflection(float);
	XMMATRIX GetReflectionViewMatrix();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX&);

	XMMATRIX m_reflectionViewMatrix;

	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;
	float moveUpDown = 0.0f;

	float camYaw = 0.0f;
	float camPitch = 0.0f;

	XMMATRIX Rotation;
	XMMATRIX Scale;
	XMMATRIX Translation;
	float rot = 0.01f;
private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMMATRIX m_viewMatrix;





	//new
	float rotx = 0;
	float rotz = 0;
	float scaleX = 1.0f;
	float scaleY = 1.0f;

	XMMATRIX Rotationx;
	XMMATRIX Rotationz;

	XMMATRIX camView;
	XMMATRIX camProjection;

	XMVECTOR camPosition;
	XMVECTOR camTarget;

	XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR DefaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);


	XMMATRIX camRotationMatrix;
	XMMATRIX groundWorld;
	
};

#endif