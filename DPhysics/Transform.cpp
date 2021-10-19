#include "pch.h"

#include "Component.h"
#include "Transform.h"

void Transform::Initialize()
{
	m_LocalMatrix = Matrix::CreateScale(m_LocalScale) * Matrix::CreateFromQuaternion(m_LocalRotation) * Matrix::CreateTranslation(m_LocalPosition);
	
	m_TransformMatrix = m_LocalMatrix * Matrix::CreateScale(m_Scale) * Matrix::CreateFromQuaternion(m_Rotation) * Matrix::CreateTranslation(m_Position);

}

void Transform::FixedUpdate(float dTime)
{
	
}

void Transform::Update()
{
	//현재 TRS 성분이 이전 TRS 성분과 다르면 TransformMatrix를 업데이트
	if (m_Position != m_Pos_Prev || m_Rotation != m_Rot_Prev)
	{
		m_TransformMatrix = m_LocalMatrix * Matrix::CreateScale(m_Scale) * Matrix::CreateFromQuaternion(m_Rotation) * Matrix::CreateTranslation(m_Position);

		m_Pos_Prev = m_Position;
		m_Rot_Prev = m_Rotation;
	}
}


void Transform::Finalize()
{

}

//helper functions
void Transform::QuaternionToEuler(const Quaternion& quat /*in*/, Vector3& angle /*out*/)
{
	float sqw = quat.w * quat.w;
	float sqx = quat.x * quat.x;
	float sqy = quat.y * quat.y;
	float sqz = quat.z * quat.z;

	float magnitude = quat.Length();

	double test = quat.x * quat.y + quat.z * quat.w;
	if (test > 0.499 * magnitude) { // singularity at north pole
		angle.y = 2 * atan2(quat.x, quat.w);
		angle.x = DirectX::XM_PIDIV2;
		angle.z = 0;
		return;
	}
	if (test < -0.499 * magnitude) { // singularity at south pole
		angle.y = -2 * atan2(quat.x, quat.w);
		angle.x = -DirectX::XM_PIDIV2;
		angle.z = 0;
		return;
	}

	angle.y = atan2f(2 * quat.y * quat.w - 2 * quat.x * quat.z, sqx - sqy - sqz + sqw);
	angle.x = asinf(2 * test / magnitude);
	angle.z = atan2f(2 * quat.x * quat.w - 2 * quat.y * quat.z, -sqx + sqy - sqz + sqw);
}

void Transform::EulerToQuaternion(const Vector3& angle /*in*/, Quaternion& quat/*out*/)
{
	double c1 = cosf(angle.y / 2);
	double s1 = sinf(angle.y / 2);
	double c2 = cosf(angle.x / 2);
	double s2 = sinf(angle.x / 2);
	double c3 = cosf(angle.z / 2);
	double s3 = sinf(angle.z / 2);
	double c1c2 = c1 * c2;
	double s1s2 = s1 * s2;

	quat.w = c1c2 * c3 - s1s2 * s3;
	quat.x = c1c2 * s3 + s1s2 * c3;
	quat.y = s1 * c2 * c3 + c1 * s2 * s3;
	quat.z = c1 * s2 * c3 - s1 * c2 * s3;
}