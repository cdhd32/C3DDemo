#include "pch.h"

#include "PhysicsEngine.h"
#include "ISimulationEventCallback.h"
#include "QueryRay.h"

bool PhysicsEngine::QueryRay::Raycast(Vector3& origin, Vector3& dir, float distance, unsigned int filterData, unsigned int filterMask, RaycastHit& raycastHit)
{
	PxSceneQueryFlags outptFlags;
	PxRaycastHit hit;
	//PxSceneQueryCache* queryCache = nullptr;

	PxQueryFilterData queryfilterData;
	//queryfilterData.data.word0 = filterData;
	//queryfilterData.data.word1 = filterMask;

	queryfilterData.flags = PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC;

	bool isHit = PhysicsEngineMain::GetInstance().RaycastSingle(ToPxVec3(origin),
		ToPxVec3(dir), distance, outptFlags,
		hit, queryfilterData, NULL
	);

	if (isHit)
	{
		//prefilter 대신 여기서 필터링 
		PxFilterData ohterFilterData = hit.shape->getQueryFilterData();

		if ((filterData != 0 || ohterFilterData.word0 != 0) &&
			!((filterData & ohterFilterData.word1) ||
				(filterMask & ohterFilterData.word0))
			)
		{
			return false;
		}

		ISimulationEventCallback* callback = reinterpret_cast<ISimulationEventCallback*>(hit.actor->userData);

		raycastHit.object = callback;
		raycastHit.worldPos = ToVector3(hit.position);
		raycastHit.worldNormal = ToVector3(hit.normal);
		raycastHit.distance = hit.distance;
	}

	return isHit;
}

bool PhysicsEngine::QueryRay::Raycast(QueryRay& ray, RaycastHit& raycastHit)
{
	return Raycast(ray.origin, ray.dir, ray.distance, ray.filterData, ray.filterMask, raycastHit);
}
