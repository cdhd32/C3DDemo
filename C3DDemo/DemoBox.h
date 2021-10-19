#pragma once
#include "../C3DEngine/pch.h"

#include "PhysicsEngine.h"
#include "PhysicsObjects.h"

class C3DShader;
class C3DMaterial;
class C3DTransform;
struct VERTEX;

//����� ����
class DemoBox : public PhysicsEngine::ISimulationEventCallback
{
protected:
	//��ġ, ���� ����

	Vector3 m_Pos = Vector3(0.f, 0.f, 0.f);
	Vector3 m_Rot = Vector3(0.f, 0.f, 0.f);
	Vector3 m_Scale = Vector3(1.f, 1.f, 1.f);

	Vector3 m_Size = Vector3(1.f, 1.f, 1.f);

	PhysicsEngine::Rigidbody* m_pRigidBody;

	//�������� ��ȯ ���
	Matrix m_WorldTM;
	Matrix m_WorldTM_priv;

	//shader
	C3DShader* m_pShader;

	// ���ؽ� ����
	ID3D11Buffer* m_pVB;

	// ���� ������ ���� ���׸���
	C3DMaterial* m_pMaterial;

	// ���� ������Ʈ��(world) ����
	C3DTransform* m_pTransform;

	//��
	VERTEX* m_pModel;

public:
	//�׽�Ʈ�� rigidboby flag ���� ����
	bool m_IsStatic = false;
	bool m_IsKenematic = false;

	DirectX::SimpleMath::Vector3 GetScale() const { return m_Scale; }
	void SetScale(DirectX::SimpleMath::Vector3 val) { m_Scale = val; }

public:
	DemoBox() = default;
	~DemoBox() = default;

	DemoBox(Vector3 pos,
		Vector3 scale,
		C3DShader* shader)
		: m_Pos(pos), m_Scale(scale),
		m_pShader(shader)
	{}

	void Initialze();
	void FixedUpdate(float dTime);
	void Update();
	void RenderUpdate();
	void Finalize();



};