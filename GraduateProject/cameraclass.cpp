////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_rotation.x = 0.0f;
	m_rotation.y = 0.0f;
	m_rotation.z = 0.0f;


	//Camera information
	camPosition = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//Set the View matrix
	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	// ���� �� ���͸� �̿��Ͽ� �� ����� �����մϴ�.
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}

void CameraClass::RenderReflection(float height)
{
	XMFLOAT3 up, position, lookAt;
	float radians;


	// ������ ����Ű�� ���͸� ����ϴ�.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// ���忡 ī�޶� ��ġ�� �����մϴ�.
	// ��� �ݻ縦 ���� ī�޶��� Y���� ������ŵ�ϴ�.
	position.x = XMVectorGetX(camPosition);
	position.y = -XMVectorGetY(camPosition) + (height * 2.0f);
	position.z = XMVectorGetZ(camPosition);

	// ȸ���� ���� ������ ����մϴ�.
	XMVECTOR rotate = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f); //���� ������̶� ���� ���� �� �����̤�
	rotate = XMVector3Transform(rotate, camRotationMatrix); //���� ������̶� ���� ���� �� �����̤�

	radians = XMVectorGetX(rotate) * 0.0174532925f;

	// ī�޶� ���� ������ �����մϴ�.
	lookAt.x = sinf(radians) + XMVectorGetX(camPosition);
	lookAt.y = position.y;
	lookAt.z = cosf(radians) + XMVectorGetZ(camPosition);

	// ���� �� ���͸� �̿��Ͽ� �� ����� �����մϴ�.
	m_reflectionViewMatrix = XMMatrixLookAtLH(camPosition, camTarget, camUp);
}

XMMATRIX CameraClass::GetReflectionViewMatrix()
{
	return m_reflectionViewMatrix;

}



XMFLOAT3 CameraClass::GetPosition()
{
	return m_position;
}


XMFLOAT3 CameraClass::GetRotation()
{
	return m_rotation;
}

// This uses the position and rotation of the camera to build and to update the view matrix.
void CameraClass::Render()
{

	// Create the rotation matrix from the yaw, pitch, and roll values.
	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);



	// Setup where the camera is looking by default.
	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);


	// Setup the vector that points upwards.

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);


	camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	camUp = XMVector3TransformCoord(DefaultUp, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

	//up = XMVector3Cross(camForward, camRight);

	camPosition += moveLeftRight * camRight;
	camPosition += moveBackForward * camForward;


	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;





	// Translate the rotated camera position to the location of the viewer.
	camTarget = camPosition + camTarget;

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	//UpdateCamera();
	return;
}


void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}