#pragma once
#include "../pch.h"

#include "GeoObjectDefine.h"

#include "../Graphics/C3DShader.h"
#include <vector>

#include "../System/C3DTransform.h"

#include "C3DGrid.h"

C3DGrid::C3DGrid(C3DShader* pShader, Vector3 centor, int wLineNum,
	int hLineNum, float intervalW, float intervalH)
	: m_pShader(pShader),
	m_CentorPos(centor),
	m_pTransform(nullptr),
	m_WidthLineNum(wLineNum),
	m_HeightLineNum(hLineNum),
	m_IntervalW(intervalW),
	m_IntervalH(intervalH),

	m_pVB(nullptr)

{
	
}

C3DGrid::~C3DGrid()
{
	m_pShader = nullptr;
}

void C3DGrid::Create()
{
	m_pTransform = new C3DTransform(m_CentorPos, Vector3(0, 0, 0), Vector3(1, 1, 1));
	//m_pTransform->Create();

	m_pLineList = new LINE[m_WidthLineNum + m_HeightLineNum];
	m_pTempVB = new VB_POS[(m_WidthLineNum + m_HeightLineNum)*2];

	int width = (int)m_IntervalW * (m_WidthLineNum - 1);
	int height = (int)m_IntervalH * (m_HeightLineNum - 1);

	//일단 xz평면(y = 0) 기준으로 라인을 정의합니다. width : x축 대응 , height : z축 대응

	//x축 -방향에서 -> +방향으로 라인 하나씩 배치
	for (int i = 0; i < m_WidthLineNum; i++)
	{
		//z축 +방향에서 -> -방향으로 선 긋기
		float xPos = m_IntervalW * i - width / 2.f + m_CentorPos.x;

		LINE line;
		line.vtx[LINE_START] = VB_POS{ xPos, m_CentorPos.y, m_CentorPos.z + (height / 2.f) };
		line.vtx[LINE_END] = VB_POS{ xPos, m_CentorPos.y, m_CentorPos.z - (height / 2.f) };
		m_pLineList[i] = line;
	}

	//z축 +방향에서 -> -방향으로 라인 하나씩 배치
	for (int i = 0; i < m_HeightLineNum; i++)
	{
		//x축 -방향에서 -> +방향으로 선 긋기
		float zPos = m_IntervalH * i - height / 2.f + m_CentorPos.z;
		LINE line;
		line.vtx[LINE_START] = VB_POS{ m_CentorPos.x - (width / 2.f), m_CentorPos.y, zPos };
		line.vtx[LINE_END] = VB_POS{ m_CentorPos.x + (width / 2.f), m_CentorPos.y, zPos };
		m_pLineList[m_WidthLineNum + i] = line;
	}

	_DataLoading();
}

void C3DGrid::Update(float dTime)
{
	//오브젝트의 worldTM 만들기
	/*XMMATRIX mTM = XMMatrixIdentity();
	XMMATRIX _mScale = XMMatrixScaling(1.f, 1.f, 1.f);
	XMMATRIX _mRot = XMMatrixRotationRollPitchYaw(0, 0, 0);
	XMMATRIX _mTrans = XMMatrixTranslation(m_CentorPos.x , m_CentorPos.y, m_CentorPos.z);
	mTM = _mScale * _mRot * _mTrans;*/

	//m_pShader->SetWorldTM(m_pTransform);

	//m_pShader->SetTransform(TS_WORLD, mTM);//worldTM 정보만 업데이트

	//임시 상수버퍼 업데이트
	//m_pShader->UpdateTempCB();

	//상수버퍼 업데이트
	//m_pShader->UpdateConstantBuffer();


}

void C3DGrid::Draw()
{
	m_pShader->SetWorldTM(m_pTransform);

	//임시 상수버퍼 업데이트
	m_pShader->UpdateTempCB();

	//상수버퍼 업데이트
	m_pShader->UpdateConstantBuffer();

	//쉐이더 set
	m_pShader->ShaderUpdate();

	//IA에 버텍스 버퍼 설정(인덱스 버퍼는 없음)
	m_pShader->SetVBtoIASimple(m_pVB, D3D11_PRIMITIVE_TOPOLOGY_LINELIST, sizeof(VB_POS));

	//그리기!
	int size = (m_WidthLineNum + m_HeightLineNum)*2;
	m_pShader->DrawFullVtx(size);
}

void C3DGrid::_DataLoading()
{
	UINT size = sizeof(LINE) * (m_WidthLineNum + m_HeightLineNum);
	size = sizeof(VB_POS) * (m_WidthLineNum + m_HeightLineNum) * 2;
	
	for (int i = 0; i < m_WidthLineNum + m_HeightLineNum; i++)
	{
		m_pTempVB[i*2] = m_pLineList[i].vtx[LINE_START];
		m_pTempVB[i*2+1] = m_pLineList[i].vtx[LINE_END];
	}

	m_pShader->CreateVB(m_pTempVB, size, &m_pVB);
}

