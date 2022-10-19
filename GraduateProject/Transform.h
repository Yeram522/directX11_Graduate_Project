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
	XMFLOAT3 localposition;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

	XMFLOAT3 parentposition;


	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
};

#endif

