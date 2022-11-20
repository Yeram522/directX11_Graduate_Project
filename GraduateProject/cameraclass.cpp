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
	XMMATRIX rotationMatrix;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);



	// Setup where the camera is looking by default.
	camTarget = XMVector3TransformCoord(DefaultForward, rotationMatrix);
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

void CameraClass::RenderReflection(float height)
{
	XMFLOAT3 up, position, lookAt;
	float radians;


	// 위쪽을 가리키는 벡터를 만듭니다.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// 월드에 카메라 위치를 설정합니다.
	// 평면 반사를 위해 카메라의 Y값을 역전시킵니다.
	position.x = XMVectorGetX(camPosition);
	position.y = -XMVectorGetY(camPosition) + (height * 2.0f);
	position.z = XMVectorGetZ(camPosition);

	// 회전을 라디안 값으로 계산합니다.
	radians = -XMVectorGetY(camRight) * 0.0174532925f;

	// 카메라가 보는 방향을 설정합니다.
	lookAt.x = sinf(radians) + XMVectorGetX(camPosition);
	lookAt.y = position.y;
	lookAt.z = cosf(radians) + XMVectorGetZ(camPosition);

	const XMFLOAT3 fup = XMFLOAT3(position.x, position.y, position.z);
	const XMFLOAT3 fposition = XMFLOAT3(lookAt.x, lookAt.y, lookAt.z);
	const XMFLOAT3 flookAt = XMFLOAT3(up.x, up.y, up.z);

	// 위의 세 벡터를 이용하여 뷰 행렬을 생성합니다.
	m_reflectionViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&fup),
		XMLoadFloat3(&fposition),
		XMLoadFloat3(&flookAt));

	return;
}
