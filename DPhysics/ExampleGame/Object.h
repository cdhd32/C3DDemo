#pragma once
#include<DirectXMath.h>
#include"EngineInterface.h"
#include"PhysicsEngine.h"
#include "PhysicsObjects.h"

using namespace DirectX;

class Object
{
protected:

	HEngine_DX12_3D* m_pHEngine;

	std::string m_name;

	Vector3 m_scale				= { 1, 1, 1 };
	Quaternion m_quatRot			= { 0, 0, 0, 1 };
	Vector3 m_translation		= { 0, 0, 0 }; 
	Matrix m_transformMatrix =
	{ 1, 0, 0, 0,
	  0, 1, 0, 0,
	  0, 0, 1, 0,
	  0, 0, 0, 1
	};

	bool m_bTransformDirty;

	//HEngine(graphic) variables
	HModelData* m_pModelData = nullptr;
	HInstanceData* m_pInstanceData = nullptr;
	HMaterialData* m_pMatData = nullptr;

	unsigned char m_shaderFlag = eNoOption_EI;




public:
	Object():m_pHEngine(HEngine_DX12_3D::GetInstance()){};
	virtual ~Object();

	virtual void Update(float dTime = 0);

	//get
	HMaterialData* GetMaterialHandle() { return m_pMatData; }
	HInstanceData* GetInstanceHandle() { return m_pInstanceData; }
	HModelData* GetModelHandle() { return m_pModelData; }
	Vector3 GetScale() { return m_scale; }
	Quaternion GetQuatRot() { return m_quatRot; }
	Vector3 GetTranslation() { return m_translation;}
	std::string GetName() { return m_name; }

	//set
	void SetModelHandle(HModelData* pModelData);
	void SetMaterialHandle(HMaterialData* pMatData);
	void SetShaderFlag(unsigned char shaderflag);
	void SetScale(Vector3 scale);
	void SetQuatRot(Quaternion quatRot);
	void SetTranslation(Vector3 translation);
	void SetTransform(Matrix transform);
	void SetName(std::string name) { m_name = name; }

	//delete HEngine Dependent resource
	void DeleteInstance();

protected:
	void UpdateTransformMatrix();

};
