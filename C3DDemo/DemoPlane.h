#pragma once
#include "../C3DEngine/pch.h"

#include "PhysicsEngine.h"
#include "PhysicsObjects.h"

class C3DShader;
class C3DMaterial;
class C3DTransform;
struct VERTEX;

//데모용 바닥
class DemoPlane : public PhysicsEngine::ISimulationEventCallback
{
public:
	static constexpr Vector3 BOXMODELSIZE = Vector3(10.f, 10.f, 10.f);
	static constexpr unsigned int BOXVTXNUM = 36;

protected:
	//위치, 물리 정보

	Vector3 m_Pos = Vector3(0.f, 0.f, 0.f);
	Vector3 m_Rot = Vector3(0.f, 0.f, 0.f);
	Vector3 m_Scale = Vector3(1.f, 1.f, 1.f);

	Vector3 m_Size = Vector3(1.f, 1.f, 1.f);

	float m_Distance; //원점과 떨어진 거리
	Vector3 m_Normal = Vector3(0.f, 1.f, 0.f); //기본 +y 방향

	PhysicsEngine::Rigidbody* m_pRigidBody;

	//렌더러용 변환 행렬
	Matrix m_WorldTM;
	Matrix m_WorldTM_priv;

	//shader
	C3DShader* m_pShader;

	// 버텍스 버퍼
	ID3D11Buffer* m_pVB;

	// 색깔 지정을 위한 마테리얼
	C3DMaterial* m_pMaterial;

	// 최종 지오메트리(world) 정보
	C3DTransform* m_pTransform;

public:
	//테스트용 rigidboby flag 변경 변수
	bool m_IsStatic = false;
	bool m_IsKenematic = false;

	DirectX::SimpleMath::Vector3 GetScale() const { return m_Scale; }
	void SetScale(DirectX::SimpleMath::Vector3 val) { m_Scale = val; }
public:
	DemoPlane() = default;
	~DemoPlane() = default;

	DemoPlane(Vector3 normal,
		float distance)
		: m_Normal(normal), m_Distance(distance)
	{}

	void Initialze();
	void FixedUpdate(float dTime);
	void Update();
	void RenderUpdate();
	void Finalize();
};

