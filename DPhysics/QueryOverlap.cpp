#include "pch.h"

#include "PhysicsEngine.h"
#include "ISimulationEventCallback.h"
#include "Shape.h"
#include "QueryOverlap.h"

physx::PxOverlapHit PhysicsEngine::QueryOverlap::HIT_BUFFER[];

const physx::PxU32 PhysicsEngine::QueryOverlap::MAX_HIT_NUM;

int PhysicsEngine::QueryOverlap::OverlapBox(Vector3 pos, Quaternion rot, Vector3 size,
	unsigned int filterData, unsigned int filterMask,
	std::vector<ISimulationEventCallback*>& hitObjects)
{

	PxBoxGeometry boxGeom;
	boxGeom.halfExtents = ToPxVec3(size / 2.f);

	PxQueryFilterData queryfilterData;
	//queryfilterData.data.word0 = filterData;
	//queryfilterData.data.word1 = filterMask;

	queryfilterData.flags = PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC;

	PxTransform tm(ToPxTransform(pos, rot));

	int overlapCnt = PhysicsEngineMain::GetInstance().OverlapMultiple( 
		boxGeom, tm, HIT_BUFFER, MAX_HIT_NUM, queryfilterData
	);


	if (overlapCnt >= 0)
	{
		for (int i = 0; i < overlapCnt; i++)
		{
			//prefilter 대신 여기서 필터링 
			PxFilterData ohterFilterData = HIT_BUFFER[i].shape->getQueryFilterData();

			if ((filterData != 0 || ohterFilterData.word0 != 0) &&
				!((filterData & ohterFilterData.word1) ||
					(filterMask & ohterFilterData.word0))
				)
			{
				continue;
			}

			ISimulationEventCallback* callback = reinterpret_cast<ISimulationEventCallback*>(HIT_BUFFER[i].actor->userData);

			hitObjects.push_back(callback);
		}
	}

	return hitObjects.size();
}

int PhysicsEngine::QueryOverlap::OverlapCapsule(Vector3 pos, Quaternion rot, float radius,
	float bodyheight, unsigned int filterData, unsigned int filterMask,
	std::vector<ISimulationEventCallback*>& hitObjects)
{
	PxCapsuleGeometry capGeom;
	capGeom.halfHeight = bodyheight / 2.f;
	capGeom.radius = radius;

	PxQueryFilterData queryfilterData;
	//queryfilterData.data.word0 = filterData;
	//queryfilterData.data.word1 = filterMask;

	queryfilterData.flags = PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC;

	PxTransform tm(ToPxTransform(pos, rot));

	int overlapCnt = PhysicsEngineMain::GetInstance().OverlapMultiple(
		capGeom, tm, HIT_BUFFER, MAX_HIT_NUM, queryfilterData
	);


	if (overlapCnt >= 0)
	{
		for (int i = 0; i < overlapCnt; i++)
		{
			//prefilter 대신 여기서 필터링 
			PxFilterData ohterFilterData = HIT_BUFFER[i].shape->getQueryFilterData();

			if ((filterData != 0 || ohterFilterData.word0 != 0) &&
				!((filterData & ohterFilterData.word1) ||
					(filterMask & ohterFilterData.word0))
				)
			{
				continue;
			}

			ISimulationEventCallback* callback = reinterpret_cast<ISimulationEventCallback*>(HIT_BUFFER[i].actor->userData);

			hitObjects.push_back(callback);
		}
	}

	return hitObjects.size();
}

int PhysicsEngine::QueryOverlap::OverlapSphere(Vector3 pos, Quaternion rot, float radius, unsigned int filterData, unsigned int filterMask, std::vector<ISimulationEventCallback*>& hitObjects)
{
	PxSphereGeometry shpereGeom;
	shpereGeom.radius = radius;

	PxQueryFilterData queryfilterData;
	//queryfilterData.data.word0 = filterData;
	//queryfilterData.data.word1 = filterMask;

	queryfilterData.flags = PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC;

	PxTransform tm(ToPxTransform(pos, rot));

	int overlapCnt = PhysicsEngineMain::GetInstance().OverlapMultiple(
		shpereGeom, tm, HIT_BUFFER, MAX_HIT_NUM, queryfilterData
	);


	if (overlapCnt >= 0)
	{
		for (int i = 0; i < overlapCnt; i++)
		{
			//prefilter 대신 여기서 필터링 
			PxFilterData ohterFilterData = HIT_BUFFER[i].shape->getQueryFilterData();

			if ((filterData != 0 || ohterFilterData.word0 != 0) &&
				!((filterData & ohterFilterData.word1) ||
					(filterMask & ohterFilterData.word0))
				)
			{
				continue;
			}

			ISimulationEventCallback* callback = reinterpret_cast<ISimulationEventCallback*>(HIT_BUFFER[i].actor->userData);

			hitObjects.push_back(callback);
		}
	}

	return hitObjects.size();
}