#include "../pch.h"

#include "../Graphics/C3DShader.h"
#include "C3DTransform.h"

C3DTransform::C3DTransform()
	:
	m_pParent(nullptr),
	m_TransMatrix(Matrix::Identity),
	m_RotMatrix(Matrix::Identity),
	m_ScaleMatrix(Matrix::Identity),
	m_TransformMatrix(Matrix::Identity)
{

}

C3DTransform::C3DTransform(const C3DTransform& trans)
	:
	m_pParent(trans.m_pParent),
	m_TransMatrix(trans.m_TransMatrix),
	m_RotMatrix(trans.m_RotMatrix),
	m_ScaleMatrix(trans.m_ScaleMatrix),
	m_TransformMatrix(trans.m_TransformMatrix)
{

}

C3DTransform::C3DTransform(Vector3 pos, Vector3 rot, Vector3 scale, C3DTransform* pParents)
	:
	m_pParent(pParents),
	m_TransMatrix(Matrix::CreateTranslation(pos)),
	m_RotMatrix(Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z)),
	m_ScaleMatrix(Matrix::CreateScale(scale)),
	m_TransformMatrix(m_ScaleMatrix * m_RotMatrix * m_TransMatrix * pParents->GetTransformMatrix())
{

}

C3DTransform::C3DTransform(Vector3 pos, Vector3 rot, Vector3 scale)
	:
	m_pParent(nullptr),
	m_TransMatrix(Matrix::CreateTranslation(pos)),
	m_RotMatrix(Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z)),
	m_ScaleMatrix(Matrix::CreateScale(scale)),
	m_TransformMatrix(m_ScaleMatrix* m_RotMatrix* m_TransMatrix)
{

}

C3DTransform::~C3DTransform()
{

}

void C3DTransform::Identity()
{
	m_pParent = nullptr;
	m_TransMatrix = Matrix::Identity;
	m_RotMatrix = Matrix::Identity;
	m_ScaleMatrix = Matrix::Identity;
	m_TransformMatrix = Matrix::Identity;
}

Vector3 C3DTransform::GetPosition()
{
	return m_TransformMatrix.Translation();
}

void C3DTransform::SetParentTransform(C3DTransform* pParent)
{
	m_pParent = pParent;
	if (m_pParent != nullptr)
	{
		m_TransformMatrix = m_ScaleMatrix * m_RotMatrix * m_TransMatrix * m_pParent->GetTransformMatrix();
	}
}

void C3DTransform::SetPosition(Vector3 pos)
{
	m_TransMatrix = Matrix::CreateTranslation(pos);
	//m_pParent가 있으면 TRS 앞에 먼저 DotProduct 한다
	if (m_pParent != nullptr)
	{
		m_TransformMatrix = m_ScaleMatrix * m_RotMatrix * m_TransMatrix * m_pParent->GetTransformMatrix();
	}
	else
	{
		///m_TransformMatrix = m_TransMatrix * m_RotMatrix * m_ScaleMatrix;
		m_TransformMatrix = m_ScaleMatrix * m_RotMatrix * m_TransMatrix;
	}
}

//Vector3 C3DTransform::GetRotation()
//{
//	//float ix = m_RotMatrix.m[][];
//	//float iy = asin(m_RotMatrix.m[2][0]);
//	//float iz = ;
//	return Vector3(ix, iy, iz);
//}

//오일러 각으로 회전
//SimpleMath의 CreateFromYawPitchRoll로 회전행렬을 만들면
//함수 내부에서는 XMMatrixRotationRollPitchYaw 함수에서 Yaw Pitch를 바꾸어서 입력한다.
//XMMatrixRotationRollPitchYaw(pitch, yaw, roll)
void C3DTransform::SetRotation(Vector3 rot)
{
	m_RotMatrix = Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
	if (m_pParent != nullptr)
	{
		m_TransformMatrix = m_ScaleMatrix * m_RotMatrix * m_TransMatrix * m_pParent->GetTransformMatrix();
	}
	else
	{
		///m_TransformMatrix = m_TransMatrix * m_RotMatrix * m_ScaleMatrix;
		m_TransformMatrix = m_ScaleMatrix * m_RotMatrix  * m_TransMatrix;

	}
}

//쿼터니온으로 회전
void C3DTransform::SetRotation(Quaternion qRot)
{
	m_RotMatrix = Matrix::CreateFromQuaternion(qRot);
	if (m_pParent != nullptr)
	{
		m_TransformMatrix = m_ScaleMatrix * m_RotMatrix * m_TransMatrix * m_pParent->GetTransformMatrix();
	}
	else
	{
		///m_TransformMatrix = m_TransMatrix * m_RotMatrix * m_ScaleMatrix;
		m_TransformMatrix = m_ScaleMatrix * m_RotMatrix * m_TransMatrix;
	}
}

//Vector3 C3DTransform::GetScale()
//{
//	return Vector3(m_ScaleMatrix._11, m_ScaleMatrix._22, m_ScaleMatrix._33);
//}

void C3DTransform::SetScale(Vector3 scale)
{
	m_ScaleMatrix = Matrix::CreateScale(scale);
	if (m_pParent != nullptr)
	{
		m_TransformMatrix =  m_ScaleMatrix * m_RotMatrix * m_TransMatrix * m_pParent->GetTransformMatrix();
	}
	else
	{
		m_TransformMatrix = m_TransMatrix * m_RotMatrix * m_ScaleMatrix;
	}
}