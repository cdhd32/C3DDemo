#pragma once
#include "DemoBox.h"

//Ű���� ��Ʈ�� ��� �߰��� �ڽ�
//���𿡼��� �ٸ� ��ü��� �浹�ϸ鼭 �������� ����� ����� ���� ������
class DemoPCBox : public DemoBox
{
protected:
	float m_Speed = 10.f;

public:

public:
	DemoPCBox() = default;
	~DemoPCBox() = default;

	DemoPCBox(Vector3 pos,
		Vector3 scale,
		C3DShader* shader)
		: DemoBox(pos, scale, shader)
	{}

	void FixedUpdate(float dTime);

};

