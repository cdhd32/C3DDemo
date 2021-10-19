#pragma once

class C3DTransform;

//���� Object Ŭ���� ���� ��� ���� ���� #����
class C3DCamera //: public C3DAlignedAllocationPolicy<16>
{
public:

protected:
	// ���� ��ȯ ����. 
	float m_fFov;
	float m_fAspect;
	float m_fZnear;
	float m_fZfar;

	// ��/ī�޶� ��ȯ ����
	// ����(200810)���� SimpleMath�� ����, ���, ���ʹϿ� �ڷ����� ����մϴ�.
	Vector3		m_vEye;
	Vector3		m_vLookAt;
	Vector3		m_vUp;

	Vector3		m_vHorz;
	Vector3		m_vView;

	//ī�޶� ���¿� ���������.
	Vector3		m_vEyeBk;
	Vector3		m_vLookAtBk;
	Vector3		m_vUpBk;

	Matrix		m_mView;
	Matrix		m_mProj;

public:
	C3DCamera();
	//�ֿ� �ɹ� ���� �ʱ�ȭ
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

