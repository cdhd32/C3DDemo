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
	//Light 임시 상수 버퍼
	//LIGHT m_Light;

	//LIGHT 사용하지 말고 카메라처럼 직접 값을 저장하게 바꾸자
	Vector3 m_Direction;   //빛의 방향.
	Vector4 m_Diffuse;     //주 광량 : 확산광 Diffuse Light.
	Vector4 m_Ambient;     //보조 광량 : 주변광 Ambient Light.
	Vector4 m_Specular;    //정반사 광량 : Specular Light.
	FLOAT  m_Range;       //빛 도달 거리.
	BOOL   m_bDiffOn;     //조명 적용여부. -> 쉐이더 교체하는 방식으로...
	BOOL   m_bSpecOn;     //정반사 적용여부. -> 쉐이더 교체하는 방식으로...

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

