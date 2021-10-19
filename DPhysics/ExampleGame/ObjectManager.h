#pragma once
#include"EngineInterface.h"
#include"ResourceManager.h"
#include"Object.h"
#include"HeroObject.h"
#include "EnemyObject.h"
#include "WeaponObject.h"

using namespace std;

class ObjectManager
{
	//오브젝트를 생성할 때 사용
	HEngine_DX12_3D* m_pHEngine;
	unique_ptr<ResourceManager> m_pResourceManager;

	//물리엔진
	PhysicsEngine::PhysicsEngineMain* m_pDPhysicEngine;

	//생성된 오브젝트 컨테이너
	vector<unique_ptr<Object>> m_objectList;
	vector<CollisionObject*> m_collisionObjectList;


public:
	static ObjectManager* GetInstance();
	void Initialize(HEngine_DX12_3D* pHEngine, PhysicsEngine::PhysicsEngineMain* pDPhysicEngine);
	void Finalize();
	void DeleteAllObject();
	

	void CreateObject();
	void Update(float dTime);
	void FixedUpdate(float accumTime);

	Object* FindObjectByName(std::string name);

private:
	ObjectManager() {};
	~ObjectManager() {};

};
