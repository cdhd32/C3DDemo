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
	//카메라 초기 위치 정보 갱신.
	m_vLookAt = lookat;
	m_vEye = eye;
	//m_vLookAt = lookat;
	//m_vEye = eye;
	Update(0);

	//카메라 리셋용 백업데이터.
	m_vLookAtBk = m_vLookAt;
	m_vEyeBk = m_vEye;
	m_vUpBk = m_vUp;
}

void C3DCamera::Update(float dTime /*= 0*/)
{
	//-----------------
	// 뷰 변환 설정 
	//-----------------
	//주인공 주시 테스트
	//g_vLookAt = XMLoadFloat3(&g_vPos);

	// 뷰 변환 행렬 생성 :  View Transform
	//Vector4 _vEye = (XMVECTOR)m_vEye;
	m_mView = XMMatrixLookAtLH(m_vEye, m_vLookAt, m_vUp);


	//-----------------
	// 투영 변환 설정.
	//----------------- 
	// 원근 투영 변환 행렬 생성 : Projection Transform.
	m_fAspect = (float)g_Mode.Width / (float)g_Mode.Height;	 //현재 해상도 기준 FOV 설정. 		
	m_mProj = XMMatrixPerspectiveFovLH(m_fFov, m_fAspect, m_fZnear, m_fZfar);


	//----------------------------
	// 상수 버퍼 갱신. -> 상수버퍼를 갖고 있는 shader에서 직접 업데이트 하기
	//----------------------------
	//g_cbDef.mView = m_mView;
	//g_cbDef.mProj = m_mProj;
}

void C3DCamera::UpdateCamera(float dTime)
{
	//리셋. #추가 #수정
	//if (IsKeyDown(VK_F9))
	//{
	//	Reset();
	//}

	static BOOL bAni = FALSE;
	if (IsKeyUp('P'))  bAni ^= TRUE;

	//카메라 자동 회전처리.
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

	// (자동회전 후) 현재 카메라 위치 교정.
	m_vView = m_vLookAt - m_vEye;
	m_vView = XMVector3Normalize(m_vView);
	m_vHorz = XMVector3Cross(m_vUp, m_vView);
	m_vHorz = XMVector3Normalize(m_vHorz);


	// 카메라 변환 미세조정 값.
	float scale = 1.0f;
	if (IsKeyDown(VK_SHIFT)) scale = 0.25f;

	//----------------------
	// 카메라 이동하기 (앞/뒤)
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
	// 카메라 회전하기.
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
	//최종 카메라 시점 계산.
	m_vEye = XMVector3TransformCoord(m_vEye, mRot);


	//----------------------
	// 카메라 행렬 갱신.
	//----------------------
	Update(dTime);	 

	//카메라 사용중 확인.
}
