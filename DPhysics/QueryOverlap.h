#pragma once

namespace PhysicsEngine
{
	struct QueryOverlap
	{
		Vector3 position;
		Quaternion rotation;
		unsigned int filterData;
		unsigned int filterMask;
	
	private:
		static const PxU32 MAX_HIT_NUM = 256;
		static PxOverlapHit HIT_BUFFER[MAX_HIT_NUM];
	public:
	
		//도형, 위치, 회전, 필터를 선택해서 overlap 함수 호출 즉시 검사를 한다.
		//선택한 도형과 겹치는 다른 오브젝트의 갯수를 최대 256개 까지 return 하고(-1를 리턴하면 256개보다 더 많은 오브젝트가 겹친 overflow 상태를 표시)
		//겹친 오브젝트들은 ISimulationEventCallback* 형태로 hitObjects(std::vector)에 저장된다.
		static int OverlapBox(Vector3 pos, Quaternion rot, Vector3 size, unsigned int filterData, unsigned int filterMask, std::vector<ISimulationEventCallback*>& hitObjects);
		static int OverlapCapsule(Vector3 pos, Quaternion rot, float radius, float bodyheight, unsigned int filterData, unsigned int filterMask, std::vector<ISimulationEventCallback*>& hitObjects);
		static int OverlapSphere(Vector3 pos, Quaternion rot, float radius, unsigned int filterData, unsigned int filterMask, std::vector<ISimulationEventCallback*>& hitObjects);
	
	};
}

