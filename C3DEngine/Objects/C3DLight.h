#pragma once

enum 
{
	LIGHT_DIRECTIONAL,
	LIGHT_POINT,
	
	_LIGHT_MAX
};

class C3DLight
{
public:
	
protected:
	//Light �ӽ� ��� ����
	//LIGHT m_Light;

	//LIGHT ������� ���� ī�޶�ó�� ���� ���� �����ϰ� �ٲ���
	Vector3 m_Direction;   //���� ����.
	Vector4 m_Diffuse;     //�� ���� : Ȯ�걤 Diffuse Light.
	Vector4 m_Ambient;     //���� ���� : �ֺ��� Ambient Light.
	Vector4 m_Specular;    //���ݻ� ���� : Specular Light.
	FLOAT  m_Range;       //�� ���� �Ÿ�.
	BOOL   m_bDiffOn;     //���� ���뿩��. -> ���̴� ��ü�ϴ� �������...
	BOOL   m_bSpecOn;     //���ݻ� ���뿩��. -> ���̴� ��ü�ϴ� �������...

public:
	C3DLight();
	~C3DLight();
	void SetUp();
	void Update();
	void Release();

	//const LIGHT& GetLight() const { return m_Light; }

	Vector3 GetDirection() const { return m_Direction; }
	void SetDirection(Vector3 val) { m_Direction = val; }
	Vector4 GetDiffuse() const { return m_Diffuse; }
	void SetDiffuse(Vector4 val) { m_Diffuse = val; }
	Vector4 GetAmbient() const { return m_Ambient; }
	void SetAmbient(Vector4 val) { m_Ambient = val; }
	Vector4 GetSpecular() const { return m_Specular; }
	void SetSpecular(Vector4 val) { m_Specular = val; }
	FLOAT GetRange() const { return m_Range; }
	void SetRange(FLOAT val) { m_Range = val; }
	BOOL GetDiffOn() const { return m_bDiffOn; }
	void SetDiffOn(BOOL val) { m_bDiffOn = val; }
	BOOL GetSpecOn() const { return m_bSpecOn; }
	void SetSpecOn(BOOL val) { m_bSpecOn = val; }
protected:
};

