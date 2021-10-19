#pragma once
class BoundingBox
{
public:
	Vector3 m_Center;
	Vector3 m_Extent;

public:
	//경계 상자 구하기(m_Center, m_Extent 계산)
	void CalculateBound(void* vtxData, UINT vcount);
};

