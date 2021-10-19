#pragma once

namespace PhysicsEngine
{
	struct QueryRay
	{
		Vector3 origin;
		Vector3 dir;
		float distance;
		unsigned int filterData;
		unsigned int filterMask;

	public:

		//방향을 가진 직선이랑 오브젝트랑 겹치는 지 검사한다.
		//먼저 hit 된(직선상 가까운) 오브젝트는 raycastHit에 맞은 위치, 방향, 거리와 같이 저장된다.
		//hit 하면 true, 안하면 false return
		static bool Raycast(Vector3& origin, Vector3& dir, float distance, unsigned int filterData, unsigned int filterMask, RaycastHit& raycastHit);
		static bool Raycast(QueryRay& ray, RaycastHit& raycastHit);
	};
}


