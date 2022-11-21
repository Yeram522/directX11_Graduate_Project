#include "Transform.h"

Transform::Transform(GameObject* gameObject):Component(gameObject)
{
	Component::gameObject = gameObject;
	m_D3D = 0;
	m_Camera = 0;
	Component::setTag("transform");
}

bool Transform::Initialize(D3DClass* m_D3D, CameraClass* m_Camera)
{
	this->m_D3D = m_D3D;
	this->m_Camera = m_Camera;

	
	return true;
}

void Transform::update()
{
	//매트릭스 초기화.
	m_Camera->GetViewMatrix(m_viewMatrix);
	m_D3D->GetWorldMatrix(m_worldMatrix);
	m_D3D->GetProjectionMatrix(m_projectionMatrix);

	if (m_BillBoard)
	{
		SetBillBoardTransform();
	}

	m_worldMatrix *= XMMatrixTranslation(localposition.x, localposition.y, localposition.z);

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


	m_worldMatrix *= XMMatrixRotationY(rotation);

}

void Transform::SetPosition(float x, float y, float z)
{
	localposition = XMFLOAT3(x, y, z);
}

void Transform::SetBillBoardTransform()
{
	double angle;
	float rotation;
	// atan2 함수를 이용하여 빌보드 모델에 적용될 회전값을 계산합니다. 이렇게 하여 빌보드 모델이 현재 카메라 위치를 바라보게 합니다.
	angle = atan2(localposition.x - m_Camera->GetPosition().x, localposition.z - m_Camera->GetPosition().z) * (180.0 / 3.14);

	// 회전각도를 라디안으로 변환합니다.
	rotation = (float)angle * 0.0174532925f;

	m_worldMatrix *= XMMatrixRotationY(rotation);
}

void Transform::FollowCamera()
{
	SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
}


