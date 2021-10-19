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
	
		//����, ��ġ, ȸ��, ���͸� �����ؼ� overlap �Լ� ȣ�� ��� �˻縦 �Ѵ�.
		//������ ������ ��ġ�� �ٸ� ������Ʈ�� ������ �ִ� 256�� ���� return �ϰ�(-1�� �����ϸ� 256������ �� ���� ������Ʈ�� ��ģ overflow ���¸� ǥ��)
		//��ģ ������Ʈ���� ISimulationEventCallback* ���·� hitObjects(std::vector)�� ����ȴ�.
		static int OverlapBox(Vector3 pos, Quaternion rot, Vector3 size, unsigned int filterData, unsigned int filterMask, std::vector<ISimulationEventCallback*>& hitObjects);
		static int OverlapCapsule(Vector3 pos, Quaternion rot, float radius, float bodyheight, unsigned int filterData, unsigned int filterMask, std::vector<ISimulationEventCallback*>& hitObjects);
		static int OverlapSphere(Vector3 pos, Quaternion rot, float radius, unsigned int filterData, unsigned int filterMask, std::vector<ISimulationEventCallback*>& hitObjects);
	
	};
}

