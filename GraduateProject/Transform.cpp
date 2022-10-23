#include "Transform.h"

Transform::Transform(GameObject* gameObject):Component(gameObject)
{
	Component::gameObject = gameObject;
	m_D3D = 0;
	m_Camera = 0;
}

bool Transform::Initialize(D3DClass* m_D3D, CameraClass* m_Camera)
{
	this->m_D3D = m_D3D;
	this->m_Camera = m_Camera;

	
	return true;
}

void Transform::Render()
{
	//매트릭스 초기화.
	m_Camera->GetViewMatrix(m_viewMatrix);
	m_D3D->GetWorldMatrix(m_worldMatrix);
	m_D3D->GetProjectionMatrix(m_projectionMatrix);
}

void Transform::Rotate()
{
	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	m_worldMatrix = XMMatrixRotationY(rotation);

}


