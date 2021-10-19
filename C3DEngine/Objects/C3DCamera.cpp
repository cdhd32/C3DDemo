#include "../pch.h"
#include "../Graphics/C3DDevice.h"

#include "../System/C3DTransform.h"

#include "C3DCamera.h"

C3DCamera::C3DCamera()
	:
	m_vEye(0.0f, 0.0f, 0.0f),
	m_vLookAt(0.f, 0.f, 0.f),
	m_vUp(0.f, 1.f, 0.f),
	m_vHorz(1.f, 0.f, 0.f),
	m_vView(0.f, 0.f, 1.f),

	m_fFov(XMConvertToRadians(60)),
	m_fAspect((float)g_Mode.Width / (float)g_Mode.Height),
	m_fZnear(1.0f),
	m_fZfar(1000.0f),

	m_vEyeBk(m_vEye),
	m_vLookAtBk(m_vLookAt),
	m_vUpBk(m_vUp),

	m_mView(XMMatrixIdentity()),
	m_mProj(XMMatrixIdentity())
{

}

C3DCamera::C3DCamera(Vector3& eye, Vector3& lookAt, Vector3 up, float fov,
	float aspect, float znear, float zfar)
	:
	m_vEye(eye),
	m_vLookAt(lookAt),
	m_vUp(up),
	m_vHorz(1.f, 0.f, 0.f),
	m_vView(0.f, 0.f, 1.f),

	m_fFov(XMConvertToRadians(fov)),
	m_fAspect(aspect),
	m_fZnear(znear),
	m_fZfar(zfar),

	m_vEyeBk(m_vEye),
	m_vLookAtBk(m_vLookAt),
	m_vUpBk(m_vUp),

	m_mView(Matrix::Identity),
	m_mProj(Matrix::Identity)
{

}

C3DCamera::~C3DCamera()
{

}

void C3DCamera::Setup(Vector3 eye, Vector3 lookat, Vector3 up)
{
	//ī�޶� �ʱ� ��ġ ���� ����.
	m_vLookAt = lookat;
	m_vEye = eye;
	//m_vLookAt = lookat;
	//m_vEye = eye;
	Update(0);

	//ī�޶� ���¿� ���������.
	m_vLookAtBk = m_vLookAt;
	m_vEyeBk = m_vEye;
	m_vUpBk = m_vUp;
}

void C3DCamera::Update(float dTime /*= 0*/)
{
	//-----------------
	// �� ��ȯ ���� 
	//-----------------
	//���ΰ� �ֽ� �׽�Ʈ
	//g_vLookAt = XMLoadFloat3(&g_vPos);

	// �� ��ȯ ��� ���� :  View Transform
	//Vector4 _vEye = (XMVECTOR)m_vEye;
	m_mView = XMMatrixLookAtLH(m_vEye, m_vLookAt, m_vUp);


	//-----------------
	// ���� ��ȯ ����.
	//----------------- 
	// ���� ���� ��ȯ ��� ���� : Projection Transform.
	m_fAspect = (float)g_Mode.Width / (float)g_Mode.Height;	 //���� �ػ� ���� FOV ����. 		
	m_mProj = XMMatrixPerspectiveFovLH(m_fFov, m_fAspect, m_fZnear, m_fZfar);


	//----------------------------
	// ��� ���� ����. -> ������۸� ���� �ִ� shader���� ���� ������Ʈ �ϱ�
	//----------------------------
	//g_cbDef.mView = m_mView;
	//g_cbDef.mProj = m_mProj;
}

void C3DCamera::UpdateCamera(float dTime)
{
	//����. #�߰� #����
	//if (IsKeyDown(VK_F9))
	//{
	//	Reset();
	//}

	static BOOL bAni = FALSE;
	if (IsKeyUp('P'))  bAni ^= TRUE;

	//ī�޶� �ڵ� ȸ��ó��.
	if (bAni)
	{
		Vector3 vPrev = m_vEye;
		Vector3 vNew = XMVectorZero();

		float angle = 3.14f * 0.05f * dTime;
		//vNew.x = sinf(angle) + vPrev.x;  		vNew.y = vPrev.y; 		vNew.z = cosf(angle) + vPrev.z;
		Matrix mRot = XMMatrixRotationY(angle);
		vNew = XMVector3TransformCoord(vPrev, mRot);
		m_vEye = vNew;
	}

	// (�ڵ�ȸ�� ��) ���� ī�޶� ��ġ ����.
	m_vView = m_vLookAt - m_vEye;
	m_vView = XMVector3Normalize(m_vView);
	m_vHorz = XMVector3Cross(m_vUp, m_vView);
	m_vHorz = XMVector3Normalize(m_vHorz);


	// ī�޶� ��ȯ �̼����� ��.
	float scale = 1.0f;
	if (IsKeyDown(VK_SHIFT)) scale = 0.25f;

	//----------------------
	// ī�޶� �̵��ϱ� (��/��)
	//----------------------
	Vector3  camPos = XMVectorZero();
	static float mov = 20.0f;
	//	if(IsKeyDown('A')) camPos -= yvCamHorz * mov * dTime;
	//	if(IsKeyDown('D')) camPos += yvCamHorz * mov * dTime;
	if (IsKeyDown('U')) camPos += m_vView * mov * dTime * scale;
	if (IsKeyDown('O')) camPos -= m_vView * mov * dTime * scale;
	//	if(IsKeyDown('R')) camPos += yvCamUp * mov * dTime;
	//	if(IsKeyDown('F')) camPos -= yvCamUp * mov * dTime;
	m_vEye = camPos + m_vEye;


	//----------------------
	// ī�޶� ȸ���ϱ�.
	//----------------------
	Vector3 camRot(0, 0, 0);
	static const float rot = XM_PI / 4.0f;
	if (IsKeyDown('I'))	camRot.x += rot * dTime * scale;
	if (IsKeyDown('K'))	camRot.x -= rot * dTime * scale;
	if (IsKeyDown('J'))	camRot.y += rot * dTime * scale;
	if (IsKeyDown('L'))	camRot.y -= rot * dTime * scale;
	//	if(IsKeyDown(DIK_INSERT)) camRot.z += rot * dTime;
	//	if(IsKeyDown(DIK_PRIOR)) camRot.z -= rot * dTime;
	Matrix mRotX, mRotY;
	mRotX = XMMatrixRotationAxis(m_vHorz, camRot.x);
	mRotY = XMMatrixRotationAxis(m_vUp, camRot.y);
	Matrix mRot = mRotX * mRotY;
	//���� ī�޶� ���� ���.
	m_vEye = XMVector3TransformCoord(m_vEye, mRot);


	//----------------------
	// ī�޶� ��� ����.
	//----------------------
	Update(dTime);	 

	//ī�޶� ����� Ȯ��.
}
