#pragma once
class BoundingBox
{
public:
	Vector3 m_Center;
	Vector3 m_Extent;

public:
	//��� ���� ���ϱ�(m_Center, m_Extent ���)
	void CalculateBound(void* vtxData, UINT vcount);
};

