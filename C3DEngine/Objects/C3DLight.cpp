#include "../pch.h"
//#include "C3DShader.h"

#include "C3DLight.h"

C3DLight::C3DLight()
	: 
	m_Direction(1, -3, 2),
	m_Diffuse(1, 1, 1, 1),
	m_Ambient(0.2f, 0.2f, 0.2f, 0.2f),
	m_Specular(1, 1, 1, 1),
	m_Range(1000.0f),
	m_bDiffOn(TRUE),
	m_bSpecOn(TRUE)
{
	m_Direction.Normalize();
}

C3DLight::~C3DLight()
{

}

void C3DLight::SetUp()
{
	
}

void C3DLight::Update()
{
	//XMVECTOR dir = XMVectorSet(1, -3, 2, 0);						//빛 방향.(4성분 주의)
	//XMStoreFloat3(&g_Light.Direction, XMVector3Normalize(-dir));	//빛 방향.(Normalize 및 방향 전환)
	//g_Light.Diffuse = XMFLOAT4A(1, 1, 1, 1);						//광원의 광량 100%
	//g_Light.Ambient = XMFLOAT4A(0.2f, 0.2f, 0.2f, 0.2f);			//주변광 광원 20%
	//g_Light.Range = 1000.0f;										//광선의 최대 도달 거리 : Directional Light 무한대.(생략가능)
	//g_Light.bDiffOn = TRUE;								//조명 On/Off.
	//g_Light.Specular = XMFLOAT4A(1, 1, 1, 1);
	//g_Light.bSpecOn = TRUE;

	/*XMVECTOR dir = XMLoadFloat3A(&m_pCBLight->Direction);
	XMStoreFloat3(&g_Light.Direction, XMVector3Normalize(-dir));
	g_Light.Diffuse = m_pCBLight->Diffuse;
	g_Light.Ambient = m_pCBLight->Ambient;
	g_Light.Specular = m_pCBLight->Specular;
	g_Light.Range = m_pCBLight->Range;
	g_Light.bDiffOn = m_pCBLight->bDiffOn;
	g_Light.bSpecOn = m_pCBLight->bSpecOn;*/

	m_Direction = Vector3(1, -3, 2);
	m_Diffuse = Vector4(1, 1, 1, 1);
	m_Ambient = Vector4(0.2f, 0.2f, 0.2f, 0.2f);
	m_Specular = Vector4(1, 1, 1, 1);
	m_Range = 1000.0f;
	m_bDiffOn = TRUE;
	m_bSpecOn = TRUE;

	m_Direction.Normalize();
}

void C3DLight::Release()
{
	
}
