#include"UnitObject.h"

void UnitObject::RotateByFrontVector(float rotateSpeed)
{
	m_curruntfront = XMFLOAT3(-m_transformMatrix._31, -m_transformMatrix._32,
		-m_transformMatrix._33);

	XMVECTOR curruntFront = XMLoadFloat3(&m_curruntfront);
	curruntFront = XMVector3Normalize(curruntFront);
	XMVECTOR destFront = XMLoadFloat3(&m_destfront);
	destFront = XMVector3Normalize(destFront);

	float angle = XMVectorGetX(XMVector3AngleBetweenNormals(curruntFront, destFront));

	if (angle > 0.01)
	{
		//������ �޼� ���� ��Ģ
		float yValueOfCrossVector = XMVectorGetY(XMVector3Cross(curruntFront, destFront));

		XMVECTOR upVector = XMVectorSet(0, 1, 0, 0);
		XMVECTOR downVector = XMVectorSet(0, -1, 0, 0);

		XMVECTOR accumulateQuatRot;

		if (yValueOfCrossVector > 0)
		{
			//y�� �߽� �ð����(������)���� ��������.
			accumulateQuatRot = XMQuaternionRotationAxis(upVector, rotateSpeed);
		}
		else if (yValueOfCrossVector < 0)
		{
			//y�� �߽� �ݽð����(����)���� ��������.
			accumulateQuatRot = XMQuaternionRotationAxis(downVector, rotateSpeed);
		}
		else
		{
			return;
		}

		XMVECTOR quatRot = XMLoadFloat4(&m_quatRot);

		quatRot = XMQuaternionMultiply(quatRot, accumulateQuatRot);

		XMStoreFloat4(&m_quatRot, quatRot);

	}
}
