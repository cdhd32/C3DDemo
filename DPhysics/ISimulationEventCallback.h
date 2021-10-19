#pragma once

namespace PhysicsEngine
{
	struct CharaterControllerHit;
	struct RaycastHit;

	class ISimulationEventCallback
	{
		friend class Rigidbody;
		friend class CharacterController;
		friend class PhysicsEngineMain;
		friend class QueryRay;
		friend class QueryOverlap;
	private:
		bool m_IsActive = true;
	public:

		//RigidActor 끼리 충돌할 때 실행
		virtual void OnCollisionEnter(ISimulationEventCallback* other) {};
		virtual void OnCollisionExit(ISimulationEventCallback* other) {};
		virtual void OnCollisionStay(ISimulationEventCallback* other) {};

		virtual void OnTriggerEnter(ISimulationEventCallback* other) {};
		virtual void OnTriggerExit(ISimulationEventCallback* other) {};
		//PhysicsEngineMain::Update에서 호출됨, 버그 때문에 사용안함
		//virtual void OnTriggerStay(ISimulationEventCallback* ohter) {};

		//CCT 끼리 충돌시 실행
		virtual void OnControllerHit(CharaterControllerHit& hit) {};

		//RayCast hit 시 실행
		//virtual void OnRaycastHit(RaycastHit& hit) {};

		//Overlap hit 시 실행
		//virtual void OnOverlapHit() {};
	};
}


