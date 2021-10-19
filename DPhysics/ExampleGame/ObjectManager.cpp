#include "ObjectManager.h"


ObjectManager* ObjectManager::GetInstance()
{
	static ObjectManager objectManager;

	return &objectManager;
}

void ObjectManager::Initialize(HEngine_DX12_3D* pHEngine, PhysicsEngine::PhysicsEngineMain* pDPhysicEngine)
{
	m_pHEngine = pHEngine;
	m_pDPhysicEngine = pDPhysicEngine;

	m_pResourceManager = std::make_unique<ResourceManager>();
	m_pResourceManager->Initialize(pHEngine);

	CreateObject();
}

void ObjectManager::Finalize()
{
	DeleteAllObject();
	m_pResourceManager->Finalize();
}

void ObjectManager::DeleteAllObject()
{
	for (unique_ptr<Object>& object : m_objectList)
	{
		object->DeleteInstance();
	}
	for (CollisionObject* object : m_collisionObjectList)
	{
		object->DeleteRigidActor();
		object->DeleteCCT();
	}

	m_objectList.clear();
	m_objectList.clear();
}

void ObjectManager::CreateObject()
{

	Matrix temp;
	temp._42 += 20;

	std::unique_ptr<HeroObject> pObject = std::make_unique<HeroObject>();
	pObject->SetModelHandle(m_pResourceManager->GetModelPointer("cylinder"));
	pObject->SetMaterialHandle(m_pResourceManager->GetMaterialPointer("metal"));
	pObject->SetTransform(temp);
	pObject->SetName("Hero");
	pObject->CreateCCTBoxFromModelBoundingBox(Vector3(0, 20, 0));

	//Weapon
	std::unique_ptr<WeaponObject> weaponCollider = std::make_unique <WeaponObject>();
	weaponCollider->SetName("Weapon");
	weaponCollider->SetShaderFlag(eNoOption_EI);
	weaponCollider->SetModelHandle(m_pResourceManager->GetModelPointer("cylinder"));
	weaponCollider->SetMaterialHandle(m_pResourceManager->GetMaterialPointer("bamboo"));
	weaponCollider->CreateRigidKinematicBox(Vector3(0, 100, 0), Vector4(0, 0, 0, 1), Vector3(20, 40, 20));
	weaponCollider->Initialize();
	weaponCollider->SetTriggerType(true);
	pObject->AttachWeapon(weaponCollider.get());

	pObject->Initialize(); 

	m_collisionObjectList.push_back(weaponCollider.get());
	m_objectList.push_back(std::move(weaponCollider));

	m_collisionObjectList.push_back(pObject.get());
	m_objectList.push_back(std::move(pObject));


	Vector3 position(0, 0, 0);
	Vector4 quaternion(0, 0, 0, 1);
	Vector3 scale(1, 1, 1);


	//맵생성 임시
	std::unique_ptr<CollisionObject> pObject2 = std::make_unique<CollisionObject>();
	pObject2->SetShaderFlag(eNoOption_EI);
	pObject2->SetModelHandle(m_pResourceManager->GetModelPointer("map"));
	pObject2->SetMaterialHandle(m_pResourceManager->GetMaterialPointer("cobble"));
	pObject2->SetName("Map");
	pObject2->CreateRigidStaticMeshFromModelData(position, quaternion, scale);
	
	m_collisionObjectList.push_back(pObject2.get());
	m_objectList.push_back(std::move(pObject2));


	std::unique_ptr<CollisionObject> collisionBox = std::make_unique<CollisionObject>();
	collisionBox->SetName("collisionBox");
	collisionBox->CreateRigidStaticBox(Vector3(-250,0,100), Vector4(0,0,0,1), Vector3(5,100,400));
	m_collisionObjectList.push_back(collisionBox.get());
	m_objectList.push_back(std::move(collisionBox));

	std::unique_ptr<CollisionObject> collisionBox2 = std::make_unique<CollisionObject>();
	collisionBox2->SetName("collisionBox2");
	collisionBox2->CreateRigidStaticBox(Vector3(200, 0, 100), Vector4(0, 0, 0, 1), Vector3(5, 100, 400));
	m_collisionObjectList.push_back(collisionBox2.get());
	m_objectList.push_back(std::move(collisionBox2));

	std::unique_ptr<CollisionObject> collisionBox3 = std::make_unique<CollisionObject>();
	collisionBox3->SetName("collisionBox3");
	collisionBox3->CreateRigidStaticBox(Vector3(0, 0, 300), Vector4(0, 0, 0, 1), Vector3(500, 100, 5));
	m_collisionObjectList.push_back(collisionBox3.get());
	m_objectList.push_back(std::move(collisionBox3));

	//create Enemy1
	std::unique_ptr<EnemyObject> EnemyBox1 = std::make_unique<EnemyObject>();
	EnemyBox1->SetName("EnemyBox1");
	EnemyBox1->SetShaderFlag(eNoOption_EI);
	EnemyBox1->SetModelHandle(m_pResourceManager->GetModelPointer("cylinder"));
	EnemyBox1->SetMaterialHandle(m_pResourceManager->GetMaterialPointer("cobble"));
	//EnemyBox1->CreateRigidDynamicBox(Vector3(50, 10, 0), Vector4(0, 0, 0, 1), Vector3(20, 20, 20));
	EnemyBox1->CreateCCTBoxFromModelBoundingBox(Vector3(50, 10, 0));
	EnemyBox1->Initialize();
	//EnemyBox1->SetTriggerType(true); //트리거 활성화
	m_collisionObjectList.push_back(EnemyBox1.get());
	m_objectList.push_back(std::move(EnemyBox1));

	//create Enemy2
	std::unique_ptr<EnemyObject> EnemyBox2 = std::make_unique<EnemyObject>();
	EnemyBox2->SetName("EnemyBox2");
	EnemyBox2->SetShaderFlag(eNoOption_EI);
	EnemyBox2->SetModelHandle(m_pResourceManager->GetModelPointer("cylinder"));
	EnemyBox2->SetMaterialHandle(m_pResourceManager->GetMaterialPointer("cobble"));
	//EnemyBox2->CreateRigidDynamicBox(Vector3(50, 10, 50), Vector4(0, 0, 0, 1), Vector3(20, 20, 20));
	EnemyBox2->CreateCCTBoxFromModelBoundingBox(Vector3(50, 10, 50));
	//EnemyBox2->m_accumMove = Vector3(0, 0, -10);
	EnemyBox2->Initialize();
	//EnemyBox2->SetTriggerType(true);
	m_collisionObjectList.push_back(EnemyBox2.get());
	m_objectList.push_back(std::move(EnemyBox2));
}

void ObjectManager::Update(float dTime)
{
	for (unique_ptr<Object>& pObject : m_objectList)
	{
		pObject->Update(dTime);
	}
}

void ObjectManager::FixedUpdate(float accumTime)
{

	for (CollisionObject* pCollisionObject : m_collisionObjectList)
	{
		pCollisionObject->FixedUpdate(accumTime);
	}
}

Object* ObjectManager::FindObjectByName(std::string name)
{
	for (unique_ptr<Object>& object : m_objectList)
	{
		if (object->GetName() == name)
			return object.get();
	}
	return nullptr;
}



