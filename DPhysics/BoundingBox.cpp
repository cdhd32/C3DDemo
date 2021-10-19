#include "pch.h"

#include "Component.h"
#include "BoundingBox.h"

//Frank Luna, Introduction to 3D Game Programming with DirectX 12, p.690

void BoundingBox::CalculateBound(void* vtxData, UINT vcount)
{
	using namespace DirectX;

	//Vector3 vMinf3(INFINITY, INFINITY, INFINITY);
	//Vector3 vMaxf3(INFINITY, INFINITY, INFINITY);

	//for (UINT i = 0; i < vcount; ++i)
	//{
	//	//Vector3 P(((float*)vtxData)[i * 3],
	//	//	((float*)vtxData)[i * 3 + 1],
	//	//	((float*)vtxData)[i * 3 + 2]);
	//	//
	//	//Vector3 shperePos;
	//	//P.Normalize(shperePos);

	//	//float theta = atan2f(shperePos.z, shperePos.x);

	//	//if (theta < 0.f)
	//	//	theta += XM_2PI;

	//	//float phi = acosf(shperePos.y);

	//	//float u = theta / (2.f * XM_PI);
	//	//float v = phi / XM_PI;

	//	//((VERTEX)vtxData)[i].TexCoord = {u , v}
	//	vMinf3 = Vector3::Min(vMinf3, P);
	//	vMaxf3 = Vector3::Max(vMaxf3, P);

	//}

	//m_Center = 0.5f * (vMin + vMax);
	//m_Extent = 0.5f * (vMax - vMin);
}
