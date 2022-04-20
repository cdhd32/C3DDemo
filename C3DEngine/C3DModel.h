#pragma once
#include "../C3DEngine/pch.h"

class C3DShader;
class C3DMaterial;
class C3DTransform;
struct VERTEX;

class C3DModel
{
public:

protected:
	//위치, 물리 정보

	Vector3 m_Pos = Vector3(0.f, 0.f, 0.f);
	Vector3 m_Rot = Vector3(0.f, 0.f, 0.f);
	Vector3 m_Scale = Vector3(1.f, 1.f, 1.f);

	Vector3 m_Size = Vector3(1.f, 1.f, 1.f);

	//렌더러용 변환 행렬
	Matrix m_WorldTM;
	Matrix m_WorldTM_priv;

	//shader
	C3DShader* m_pShader = nullptr;

	// 버텍스 버퍼
	ID3D11Buffer* m_pVB = nullptr;

	// 색깔 지정을 위한 마테리얼
	C3DMaterial* m_pMaterial = nullptr;

	// 최종 지오메트리(world) 정보
	C3DTransform* m_pTransform = nullptr;

	//모델
	VERTEX* m_pModel = nullptr;

public:
	C3DModel() = default;
	virtual ~C3DModel() = default;

	C3DModel(Vector3 pos,
		Vector3 scale,
		C3DShader * shader)
		: m_Pos(pos), m_Scale(scale),
		m_pShader(shader)
	{}

	void Initialze();
	void FixedUpdate(float dTime);
	void Update();
	void RenderUpdate();
	void Finalize();
};

