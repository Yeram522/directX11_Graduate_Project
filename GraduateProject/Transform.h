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

class Transform 
{
public:
	Transform();
	bool Initialize(D3DClass*, CameraClass*);
	void Render();
	void Rotate();

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

