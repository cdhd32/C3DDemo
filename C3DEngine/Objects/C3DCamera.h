#pragma once

class C3DTransform;

//향후 Object 클래스 만들어서 상속 받을 예정 #수정
class C3DCamera //: public C3DAlignedAllocationPolicy<16>
{
public:

protected:
	// 투영 변환 정보. 
	float m_fFov;
	float m_fAspect;
	float m_fZnear;
	float m_fZfar;

	// 뷰/카메라 변환 정보
	// 이제(200810)부터 SimpleMath의 벡터, 행렬, 쿼터니온 자료형을 사용합니다.
	Vector3		m_vEye;
	Vector3		m_vLookAt;
	Vector3		m_vUp;

	Vector3		m_vHorz;
	Vector3		m_vView;

	//카메라 리셋용 백업데이터.
	Vector3		m_vEyeBk;
	Vector3		m_vLookAtBk;
	Vector3		m_vUpBk;

	Matrix		m_mView;
	Matrix		m_mProj;

public:
	C3DCamera();
	//주요 맴버 직접 초기화
	//C3DCamera(Vector3& eye, Vector3& lookAt, float fov, float aspect, float znear, float zfar);
	C3DCamera(Vector3& eye, Vector3& lookAt, Vector3 up, float fov, float aspect, float znear, float zfar);
	~C3DCamera();

	void Setup(Vector3 eye, Vector3 lookat, Vector3 up);	//start or init virtual
	void Update(float dTime = 0);	//virtual
	void UpdateCamera(float dTime = 0);

	const Matrix& GetViewMatrix() const { return m_mView; };
	const Matrix& GetProjMatrix() const { return m_mProj; };
protected:

};

