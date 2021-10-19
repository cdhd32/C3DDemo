
#include"CollisionObject.h"

using namespace PhysicsEngine;

CollisionObject::~CollisionObject()
{
	
}

void CollisionObject::CreateRigidStaticMeshFromModelData(Vector3 position, Quaternion rotation, Vector3 scale)
{
	

	if (m_pModelData == nullptr)
		throw;

	HModelRawData const* pRawData = m_pModelData->pRawData;

	PhysicsEngine::TriangleMeshData triangleMesh = {};
	std::vector<float> vertexBuffer;
	vertexBuffer.reserve(pRawData->vertexData.size() * 3);
	std::vector<PxU32> indexBuffer;
	vertexBuffer.reserve(pRawData->indexData.size());

	//fbx로 로드한 모델 데이터를 기반해서 TRIANGLEMESH를 구조체를 채워준다.
	{

		for (int i = 0; i < pRawData->vertexData.size()/3; i++)
		{
			//position.push_back(pHmodelRawData->vertexData[3 * i].pos.x);
			//position.push_back(pHmodelRawData->vertexData[3 * i].pos.y);
			//position.push_back(pHmodelRawData->vertexData[3 * i].pos.z);

			//position.push_back(pHmodelRawData->vertexData[3 * i + 1].pos.x);
			//position.push_back(pHmodelRawData->vertexData[3 * i + 1].pos.y);
			//position.push_back(pHmodelRawData->vertexData[3 * i + 1].pos.z);

			//position.push_back(pHmodelRawData->vertexData[3 * i + 2].pos.x);
			//position.push_back(pHmodelRawData->vertexData[3 * i + 2].pos.y);
			//position.push_back(pHmodelRawData->vertexData[3 * i + 2].pos.z);

			vertexBuffer.push_back(pRawData->vertexData[3 * i + 2].pos.x);
			vertexBuffer.push_back(pRawData->vertexData[3 * i + 2].pos.y);
			vertexBuffer.push_back(pRawData->vertexData[3 * i + 2].pos.z);


			vertexBuffer.push_back(pRawData->vertexData[3 * i + 1].pos.x);
			vertexBuffer.push_back(pRawData->vertexData[3 * i + 1].pos.y);
			vertexBuffer.push_back(pRawData->vertexData[3 * i + 1].pos.z);

			vertexBuffer.push_back(pRawData->vertexData[3 * i].pos.x);
			vertexBuffer.push_back(pRawData->vertexData[3 * i].pos.y);
			vertexBuffer.push_back(pRawData->vertexData[3 * i].pos.z);
		}

		for (int i = 0; i < pRawData->indexData.size(); i++)
		{
			indexBuffer.push_back(pRawData->indexData[i]);
		}

		triangleMesh.verts = vertexBuffer.data();
		triangleMesh.vertNum = vertexBuffer.size() / 3;
		triangleMesh.faces = indexBuffer.data();
		triangleMesh.faceNum = indexBuffer.size() / 3;
	}

	//그래픽툴 좌표계에서 dx좌표계로 바꿔주는 행렬을 이용해 SRT정보를 얻는다
	{
		Matrix transformMatrix;
		transformMatrix *= Matrix::CreateScale(scale);
		transformMatrix *= Matrix::CreateFromQuaternion(rotation);
		transformMatrix *= Matrix::CreateTranslation(position);

		m_axisModify = pRawData->AxisSystempModify;

		transformMatrix *= m_axisModify;

		transformMatrix.Decompose(m_axisModifyScale, m_axisModifyQuat, m_axisModifyTranslation);

	}

	Shape* pTriangleShape = new TriangleMeshShape(*m_p3DPhysicsEngine, triangleMesh, m_axisModifyScale, Vector3::Zero);

	m_pRigidbody = new Rigidbody(*m_p3DPhysicsEngine, this,
		pTriangleShape, RigidbodyType::Static, m_axisModifyTranslation, m_axisModifyQuat);

	m_pRigidbody->Initialize();

	//m_pRigidActor = PhysicsEngineInterface::CreateStaticActorFromMeshData(triangleMesh, m_axisModify);

	m_collsionType = CollisionType::eRigidStatic;

	/*PhysicsEngineInterface::SetupCollisionFilter(m_pRigidActor, PhysicsEngineInterface::FILTERGROUP::eFIELD,
		PhysicsEngineInterface::FILTERGROUP::Enum(PhysicsEngineInterface::FILTERGROUP::ePC | PhysicsEngineInterface::FILTERGROUP::eENEMY));*/
}

void CollisionObject::CreateCCTBoxFromModelBoundingBox(Vector3 initPos)
{
	if (m_pModelData == nullptr)
		throw;

	HModelRawData const* pRawData = m_pModelData->pRawData;

	m_center = pRawData->boundingBox.Center;
	m_translation = initPos;
	m_pCCT = CharacterController::CreateCapsule(this, initPos + m_center,
		pRawData->boundingBox.Extents.x / 2, pRawData->boundingBox.Extents.y);

	m_collsionType = CollisionType::eCharacterControl;
}

void CollisionObject::CreateRigidStaticBox(Vector3 position, Quaternion rotation, Vector3 size)
{

	m_pRigidbody = Rigidbody::CreateBox(this, RigidbodyType::Static, position, rotation, size);

	m_collsionType = CollisionType::eRigidStatic;
}

void CollisionObject::CreateRigidKinematicBox(Vector3 position, Quaternion rotation, Vector3 size)
{
	m_pRigidbody = Rigidbody::CreateBox(this, RigidbodyType::Kinematic, position, rotation, size);

	m_collsionType = CollisionType::eRigidKinematic;
}

void CollisionObject::CreateRigidDynamicBox(Vector3 position, Quaternion rotation, Vector3 size)
{
	m_pRigidbody = Rigidbody::CreateBox(this, RigidbodyType::Dynamic, position, rotation, size);

	m_collsionType = CollisionType::eRigidDynamic;
}

void CollisionObject::DeleteRigidActor()
{
	if (m_pRigidbody)
	{
		m_pRigidbody->Release();
		delete m_pRigidbody;
		m_pRigidbody = nullptr;
	}
}

void CollisionObject::DeleteCCT()
{
	if (m_pCCT)
	{
		m_pCCT->Release();
		delete m_pCCT;
		m_pCCT = nullptr;
	}
}

void CollisionObject::FixedUpdate(float accumTime)
{
	switch (m_collsionType)
	{
	case CollisionType::eNone:
	case CollisionType::eRigidStatic:
		break;
	case CollisionType::eCharacterControl:
		if(m_pCCT)
		{		
			if(!m_pCCT->IsReleased())
				m_collisionDir = m_pCCT->UpdatePosition(m_accumTranslation, accumTime);
		}
		break;
	case CollisionType::eRigidKinematic:
	case CollisionType::eRigidDynamic:
		if (m_pRigidbody)
		{
			if(!m_pRigidbody->IsReleased())
				m_pRigidbody->UpdateTransformFixed(m_translation, m_quatRot, accumTime);
		}
		break;
	default:
		break;
	}

	m_accumTranslation = Vector3::Zero;
}

void CollisionObject::Update(float dTime)
{
	Vector3 position;
	Quaternion rotation;

	switch (m_collsionType)
	{
	case CollisionType::eNone:
		break;
	case CollisionType::eCharacterControl:
		m_pCCT->GetPosition(position);
		position = position - m_center;
		SetTranslation(position);
		break;
	case CollisionType::eRigidStatic:
		break;
	case CollisionType::eRigidKinematic:
	case CollisionType::eRigidDynamic:
		m_pRigidbody->GetTransform(position, rotation);
		position = position - m_center;
		SetTranslation(position);
		SetQuatRot(rotation);
		break;
	default:
		break;
	}

	Object::UpdateTransformMatrix();
}

void CollisionObject::SetTriggerType(bool isTrigger)
{
	m_pRigidbody->SetTrigger(isTrigger);
}

void CollisionObject::SetAtive(bool isActive)
{
	m_pRigidbody->SetActive(isActive);
}

void CollisionObject::SetFilterGroup(unsigned int filterGroup, unsigned int targetGroup)
{
	if (m_pCCT)
	{
		m_pCCT->SetupCollisionFilter(filterGroup, targetGroup);
	}
	else
	{
		m_pRigidbody->SetupCollisionFilter(filterGroup, targetGroup);
	}
}

void CollisionObject::OnCollisionEnter(ISimulationEventCallback* ohter)
{
	OnCollisionEnter(dynamic_cast<CollisionObject*>(ohter));
}

void CollisionObject::OnCollisionExit(ISimulationEventCallback* ohter)
{
	OnCollisionExit(dynamic_cast<CollisionObject*>(ohter));
}

void CollisionObject::OnCollisionStay(ISimulationEventCallback* ohter)
{
	OnCollisionStay(dynamic_cast<CollisionObject*>(ohter));
}

void CollisionObject::OnTriggerEnter(ISimulationEventCallback* ohter)
{
	OnTriggerEnter(dynamic_cast<CollisionObject*>(ohter));
}

void CollisionObject::OnTriggerExit(ISimulationEventCallback* ohter)
{
	OnTriggerExit(dynamic_cast<CollisionObject*>(ohter));
}

void CollisionObject::OnTriggerStay(PhysicsEngine::ISimulationEventCallback* ohter)
{
	OnTriggerStay(dynamic_cast<CollisionObject*>(ohter));
}

void CollisionObject::OnControllerHit(PhysicsEngine::CharaterControllerHit& hit)
{
	OnControllerHitCollision(hit);
}
