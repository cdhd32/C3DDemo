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

	//�ϴ� xz���(y = 0) �������� ������ �����մϴ�. width : x�� ���� , height : z�� ����

	//x�� -���⿡�� -> +�������� ���� �ϳ��� ��ġ
	for (int i = 0; i < m_WidthLineNum; i++)
	{
		//z�� +���⿡�� -> -�������� �� �߱�
		float xPos = m_IntervalW * i - width / 2.f + m_CentorPos.x;

		LINE line;
		line.vtx[LINE_START] = VB_POS{ xPos, m_CentorPos.y, m_CentorPos.z + (height / 2.f) };
		line.vtx[LINE_END] = VB_POS{ xPos, m_CentorPos.y, m_CentorPos.z - (height / 2.f) };
		m_pLineList[i] = line;
	}

	//z�� +���⿡�� -> -�������� ���� �ϳ��� ��ġ
	for (int i = 0; i < m_HeightLineNum; i++)
	{
		//x�� -���⿡�� -> +�������� �� �߱�
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
	//������Ʈ�� worldTM �����
	/*XMMATRIX mTM = XMMatrixIdentity();
	XMMATRIX _mScale = XMMatrixScaling(1.f, 1.f, 1.f);
	XMMATRIX _mRot = XMMatrixRotationRollPitchYaw(0, 0, 0);
	XMMATRIX _mTrans = XMMatrixTranslation(m_CentorPos.x , m_CentorPos.y, m_CentorPos.z);
	mTM = _mScale * _mRot * _mTrans;*/

	//m_pShader->SetWorldTM(m_pTransform);

	//m_pShader->SetTransform(TS_WORLD, mTM);//worldTM ������ ������Ʈ

	//�ӽ� ������� ������Ʈ
	//m_pShader->UpdateTempCB();

	//������� ������Ʈ
	//m_pShader->UpdateConstantBuffer();


}

void C3DGrid::Draw()
{
	m_pShader->SetWorldTM(m_pTransform);

	//�ӽ� ������� ������Ʈ
	m_pShader->UpdateTempCB();

	//������� ������Ʈ
	m_pShader->UpdateConstantBuffer();

	//���̴� set
	m_pShader->ShaderUpdate();

	//IA�� ���ؽ� ���� ����(�ε��� ���۴� ����)
	m_pShader->SetVBtoIASimple(m_pVB, D3D11_PRIMITIVE_TOPOLOGY_LINELIST, sizeof(VB_POS));

	//�׸���!
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

