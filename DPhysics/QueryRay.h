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

		//������ ���� �����̶� ������Ʈ�� ��ġ�� �� �˻��Ѵ�.
		//���� hit ��(������ �����) ������Ʈ�� raycastHit�� ���� ��ġ, ����, �Ÿ��� ���� ����ȴ�.
		//hit �ϸ� true, ���ϸ� false return
		static bool Raycast(Vector3& origin, Vector3& dir, float distance, unsigned int filterData, unsigned int filterMask, RaycastHit& raycastHit);
		static bool Raycast(QueryRay& ray, RaycastHit& raycastHit);
	};
}


