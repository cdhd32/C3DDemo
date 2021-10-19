#include "Object.h"

void Object::SetModelHandle(HModelData* pModelData)
{
	if (m_pModelData != pModelData)
	{
		if (m_pInstanceData == nullptr)
		{
			m_pInstanceData = pModelData->AddInstance(m_shaderFlag);
		}
		else
		{
			m_pInstanceData->Delete();
			m_pInstanceData = pModelData->AddInstance(m_shaderFlag);
		}
		m_pModelData = pModelData;
	}
}

void Object::SetMaterialHandle(HMaterialData* pMatData)
{
	if (m_pMatData != pMatData)
	{
		m_pMatData = pMatData;

		if (m_pInstanceData != nullptr)
		{
			m_pInstanceData->SetMaterial(m_pMatData, 0);
		}
	}
}

void Object::SetTransform(Matrix transform)
{
	m_transformMatrix = transform;
	m_transformMatrix.Decompose(m_scale, m_quatRot, m_translation);
	m_bTransformDirty = true;
}

void Object::SetShaderFlag(unsigned char shaderflag)
{
	if (m_shaderFlag == shaderflag)
		return;

	if (m_pModelData == nullptr)
	{
		m_shaderFlag = shaderflag;
		return;
	}

	m_shaderFlag = shaderflag;
	m_pInstanceData->Delete();
	m_pInstanceData = m_pModelData->AddInstance(m_shaderFlag);
}

void Object::DeleteInstance()
{
	if (m_pInstanceData != nullptr)
	{
		m_pInstanceData->Delete();
		m_pInstanceData = nullptr;
	}
}

void Object::SetScale(Vector3 scale)
{
	m_scale = scale;
	m_bTransformDirty = true;
}

void Object::SetQuatRot(Quaternion quatRot)
{
	m_quatRot = quatRot;
	m_bTransformDirty = true;

}
void Object::SetTranslation(Vector3 translation)
{
	m_translation = translation;
	m_bTransformDirty = true;
}

void Object::Update(float dTime)
{
	if (m_bTransformDirty == true)
	{
		UpdateTransformMatrix();
		m_bTransformDirty = false;
	}
}

void Object::UpdateTransformMatrix()
{
	Matrix trasformMatrix;
	trasformMatrix *= Matrix::CreateScale(m_scale);
	trasformMatrix *= Matrix::CreateFromQuaternion(m_quatRot);
	trasformMatrix *= Matrix::CreateTranslation(m_translation);

	m_transformMatrix = trasformMatrix;

	if (m_pInstanceData != nullptr)
	{
		m_pInstanceData->worldTM = m_transformMatrix;
	}
}

Object::~Object()
{
}