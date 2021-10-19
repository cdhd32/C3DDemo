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

		//RigidActor ���� �浹�� �� ����
		virtual void OnCollisionEnter(ISimulationEventCallback* other) {};
		virtual void OnCollisionExit(ISimulationEventCallback* other) {};
		virtual void OnCollisionStay(ISimulationEventCallback* other) {};

		virtual void OnTriggerEnter(ISimulationEventCallback* other) {};
		virtual void OnTriggerExit(ISimulationEventCallback* other) {};
		//PhysicsEngineMain::Update���� ȣ���, ���� ������ ������
		//virtual void OnTriggerStay(ISimulationEventCallback* ohter) {};

		//CCT ���� �浹�� ����
		virtual void OnControllerHit(CharaterControllerHit& hit) {};

		//RayCast hit �� ����
		//virtual void OnRaycastHit(RaycastHit& hit) {};

		//Overlap hit �� ����
		//virtual void OnOverlapHit() {};
	};
}


