#pragma once
#include "Rigidbody.h"

namespace PhysicsEngine
{
	class Trigger : public Rigidbody
	{
	public:
		Trigger(PhysicsEngineMain& physicsEngine,
			Shape* pShape,
			RigidbodyType type,
			Vector3 worldPos = Vector3(0, 0, 0),
			Quaternion worldRot = Quaternion::Identity)
			:
			Rigidbody(physicsEngine, pShape, type, worldPos, worldRot)
		{}

		~Trigger() = default;

	public:
		static Trigger* CreateBox(RigidbodyType type, Vector3 position, Quaternion rotation, Vector3 size, Vector3 centerPos = Vector3::Zero);
		static Trigger* CreateCapsule(RigidbodyType type, Vector3 position, Quaternion rotation, float radius, float bodyHeight, Vector3 centerPos = Vector3::Zero);
		static Trigger* CreateSphere(RigidbodyType type, Vector3 position, Quaternion rotation, float radius, Vector3 centerPos = Vector3::Zero);
	
	private:
		//사용 못하게 막아주기
		static Rigidbody* CreatePlaneStatic(Vector3 normal, float distance) {};
		static Rigidbody* CreateTriangleMeshStatic(const TriangleMeshData& mesh, Vector3 position, Quaternion rotation, Vector3 scale, Vector3 centerPos = Vector3::Zero) {};
	};
}


