#pragma once

typedef LONG_PTR RRIGIDACTOR_PTR;
typedef LONG_PTR CCT_PTR;

//Physics Actor summery
// RigidActor ┌ RigidStatic
//			  ├ RigidDynamic
//			  └ RigidKinematic(has different flags RigidDynamic)
//Character Controller (CCT) has a RigidKinematic

enum PHYSICS_SHAPE_ENUM
{
	//can create static/dynamic actor
	eSHAPE_BOX,
	eSHAPE_CAPSULE,
	eSHAPE_SPHERE,
	
	//can create static actor, don't create dynamic actor
	eSHAPE_TRIANGLEMESH,
	eSHAPE_QUAD,

	eSHAPE_PLANE,

	eSHAPE_NONE
};

class PhysicsEngine::ISimulationEventCallback;

class CollisionObject;

namespace PhysicsEngineInterface
{
	using namespace DirectX::SimpleMath;
	using namespace physx;

	struct FILTERGROUP
	{
		enum Enum
		{
			ePC = (1 << 0),
			eENEMY = (1 << 1),
			eFIELD = (1 << 2),
			eWEAPON = (1 << 3)
		};
	};

	struct CCT_COLLISION_DIR
	{
		enum Enum
		{
			NONE = (1 << 0),
			UP = (1 << 1),
			DOWN = (1 << 2),
			SIDE = (1 << 3)
		};
	};

	struct TRIANGLEMESH
	{
		PxU32 vertNum;
		PxU32 faceNum;
		const float* verts;
		const PxU32* faces;
	};

	struct QUADVTX {
		Vector3 topLeft;
		Vector3	topRight;
		Vector3 bottomLeft;
		Vector3 bottomRight;
	};

	//임시 밀도(density) 상수
	static const PxReal DEFAULT_DENSITY = 10.0f;


	static const QUADVTX QUADVTXDATA = {
		Vector3(-0.5f, 0.5f, 0),
		Vector3(0.5f, 0.5f, 0),
		Vector3(-0.5f, -0.5f, 0),
		Vector3(0.5f, -0.5f, 0)
	};

	const unsigned int QUADFACEDATA[6] = { 0, 1, 2,
												3, 2, 1 };

	const unsigned int QUADVERTNNUM = 6;

	const unsigned int QUADFACENUM = 2;

	//Create RigidActor
	RRIGIDACTOR_PTR CreateRigidActorBox(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, Vector3 size, bool isStatic, bool isKenematic);
	RRIGIDACTOR_PTR CreateRigidActorSphere(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, float radius, bool isStatic, bool isKenematic);
	RRIGIDACTOR_PTR CreateRigidActorCapsule(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, float radius, float halfHeight, bool isStatic, bool isKenematic);
	
	RRIGIDACTOR_PTR CreateRigidStaticBox(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, Vector3 size);
	RRIGIDACTOR_PTR CreateRigidDynamicBox(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, Vector3 size);
	RRIGIDACTOR_PTR CreateRigidKinematicBox(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, Vector3 size);
	
	RRIGIDACTOR_PTR CreateRigidStaticCapsule(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, float radius, float halfHeight);
	RRIGIDACTOR_PTR CreateRigidDynamicCapsule(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, float radius, float halfHeight);
	RRIGIDACTOR_PTR CreateRigidKinematicCapsule(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, float radius, float halfHeight);
	
	RRIGIDACTOR_PTR CreateRigidStaticSphere(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, float radius);
	RRIGIDACTOR_PTR CreateRigidDynamicSphere(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, float radius);
	RRIGIDACTOR_PTR CreateRigidKinematicSphere(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, float radius);
	//PxTransform 테스트용 함수
	RRIGIDACTOR_PTR CreateStaticActorFromMeshData(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, const TRIANGLEMESH& meshdata, Matrix mat);
	RRIGIDACTOR_PTR CreateStaticActorFromMeshData(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, const TRIANGLEMESH& meshdata, Vector3 position, Quaternion rotation, Vector3 scale);
	
	RRIGIDACTOR_PTR CreateRigidStaticQuad(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, Vector2 size);
	
	RRIGIDACTOR_PTR CreateRigidStaticPlane(Vector3 normal, float distance);

	//RRIGIDACTOR의 Shape 변경 함수
	//비용이 많이 드는 작업이므로 툴에서 사용하는 것을 추천
	void SetSizeBox(const RRIGIDACTOR_PTR& dest, Vector3 size);
	void SetSizeSphere(const RRIGIDACTOR_PTR& dest, float radius);
	void SetSizeCapsule(const RRIGIDACTOR_PTR& dest, float radius, float halfHeight);
	void SetScaleTriangleMesh(const RRIGIDACTOR_PTR& dest, Vector3 scale);
	//void SetSizeQuad(const RRIGIDACTOR_PTR& dest, Vector2 size);

	//void SetPlane(const RRIGIDACTOR_PTR& dest, Vector3 normal, float distance);

	//set RigidActor's Shape
	//
	void SetTriggerShape(const RRIGIDACTOR_PTR& dest, bool isTrigger);
	void SetSimulationShape(const RRIGIDACTOR_PTR& dest, bool isSimulated);
	void SetTriggerShapeCCT(const CCT_PTR& dest, bool isTrigger);
	void SetSimulationShapeCCT(const CCT_PTR& dest, bool isSimulated);

	//set Actor DISABLE_SIMULATION flag
	void SetActive(const RRIGIDACTOR_PTR& dest, bool isActive);
	void SetActiveCCT(const CCT_PTR& dest, bool isActive);

	//Update RigidActor(position, rotation only)
	//recommend use before excution PhysicsEngineMain::Update()
	void UpdateRigidDynamic(const RRIGIDACTOR_PTR& dest, Vector3 position, Vector3 rotation);
	void UpdateRigidDynamic(const RRIGIDACTOR_PTR& dest, Vector3 position, Quaternion rotation);

	void UpdateRigidKinematic(const RRIGIDACTOR_PTR& dest, Vector3 position, Vector3 rotation);
	void UpdateRigidKinematic(const RRIGIDACTOR_PTR& dest, Vector3 position, Quaternion rotation);

	//Get RigidActor's Transform(position, rotation only)
	//recommend use after excution PhysicsEngineMain::Update()
	void GetTransformRigidActor(const RRIGIDACTOR_PTR& dest, Vector3& position, Vector3& rotation);
	void GetTransformRigidActor(const RRIGIDACTOR_PTR& dest, Vector3& position, Quaternion& rotation);

	//Delete RigidActor
	void DeleteRigidActor(RRIGIDACTOR_PTR& dest);

	//Create Character Controller (CCT)
	//Choose only shape BOX or CAPSULE
	//CCT_PTR CreateCCTBox(Vector3 initPos, float width, float height);
	CCT_PTR CreateCCTBox(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, Vector3 initPos, Vector3 halfExtent);
	CCT_PTR CreateCCTCapsule(PhysicsEngine::ISimulationEventCallback* pOwnerCallback, Vector3 initPos, float radius, float height);
	
	//Update CCT
	//recommend use before excution PhysicsEngineMain::Update()
	//+Return Collision Direction(Up, Down, Side, None)
	CCT_COLLISION_DIR::Enum UpdatePositionCCT(const CCT_PTR dest, Vector3 velocity, float dTime);

	//Get CCT Position
	//recommend use after excution PhysicsEngineMain::Update()
	void GetPositionCCT(const CCT_PTR dest, Vector3& position);
	void GetFootPositionCCT(const CCT_PTR dest, Vector3& position);

	//Delete CCT
	void DeleteCCT(const CCT_PTR dest);

	void SetupCollisionFilterCCT(const CCT_PTR dest, FILTERGROUP::Enum filterGroup, FILTERGROUP::Enum filterMask);
	void SetupCollisionFilter(const RRIGIDACTOR_PTR dest, FILTERGROUP::Enum filterGroup, FILTERGROUP::Enum filterMask);
}
