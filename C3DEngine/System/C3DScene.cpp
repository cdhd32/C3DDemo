#include "../pch.h"

#include "../Graphics/C3DDevice.h"
#include "../Graphics/C3DShader.h"

#include "../Objects/C3DCamera.h"
#include "../Objects/C3DLight.h"
#include "../Objects/C3DMaterial.h"

#include "../System/C3DTimer.h"

#include "../Objects/C3DGrid.h"
#include "../Objects/C3DObject.h"

#include "../../CubePlanet/GameSrc/Cube.h"
#include "../../CubePlanet/GameSrc/RCube.h"
#include "../../CubePlanet/RubiksCube.h"

#include "C3DScene.h"

C3DScene::C3DScene()
	:m_SceneId(0), m_SceneName(L"")
{

}

C3DScene::~C3DScene()
{

}

// �ʱ�ȭ �Լ�, Scene ���� ����� ����� �ִ´ٸ� m_SceneFileName�� �����ϰ� Ȯ���Ѵ�
// scene ���� �̸��� sceneName�� ���� �̸����� �����Ѵ�. 
//void C3DScene::Initialize(std::wstring sceneName)
//{
//	m_SceneName = sceneName;
//	//_CreateObjects();
//	//_LaodData();
//}

//hWnd ���޿� scene test �Լ�
void C3DScene::Initialize(std::wstring sceneName, HWND hWnd)
{
//	m_SceneName = sceneName;
//	
//	_CreateObjects(hWnd);
//	_LoadData();
//
//	//�ε� ������ BGM ��� ����, �ּ� �����ϸ� ���� �۵���
//	//m_SoundEngine.PlayBGMSound(eSoundResourceBGM::INGAME);
//	m_SoundEngine.SetBGMVolume(0.8);
}

void C3DScene::FixedUpdate()
{
}

// Manager���� ������Ʈ�ϴ� �Լ�
void C3DScene::Update()
{
	////�� ������Ʈ �� dTime�� �����´�.
	//float dTime = C3DTimer::GetInstanse()->GetDeltaTime();
	//
	////test code -> ���� ������ ��ӹ��� Ŭ��������...

	////���� ���� ������Ʈ, ���� ���� �ٸ� ������ ȣ���ϴ� �� ���� �� ����. #����
	//m_pDevice->RenderStateUpdate();

	////�� ������Ʈ ������Ʈ

	////ObjUpdate(dTime);
	//m_pCam->UpdateCamera(dTime);
	//m_pDirLight->Update();
	////m_pMtrl->Update();

	////�ӽ� ������� ������Ʈ, cam, light, material �� ��� ���� ���� 
	//m_pShader->UpdateTempCB();

	////������� ������Ʈ(GPU)
	//m_pShader->UpdateConstantBuffer();

	////������Ʈ�� worldTM ����� -> ������Ʈ ���� ����
	///*XMMATRIX mTM = XMMatrixIdentity();
	//XMMATRIX _mScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//XMMATRIX _mRot = XMMatrixRotationRollPitchYaw(0, 0, 0);
	//XMMATRIX _mTrans = XMMatrixTranslation(0, 0, 0);
	//mTM = _mScale * _mRot * _mTrans;*/

	////m_pShader->SetTramsform2();// ��������� view, proj �� ���� 

	////m_pShader->SetTransform(TS_WORLD, mTM);//������Ʈ worldTM ������ ������Ʈ

	////�׸��� ������Ʈ
	//m_pGrid->Update(dTime);

	////ť�� ������Ʈ
	/////m_pRCube->Update(dTime);
	////m_pRubiksCube->Update(dTime);

	//
	////�� ������Ʈ ������Ʈ ��


}

// Manager���� Render�ϴ� �Լ�
void C3DScene::Render()
{
	////test code -> ���� ������ ��ӹ��� Ŭ��������...
	////����� Ŭ����
	//m_pDevice->RTClear(C3DDevice::CLEAR_COLOR_DBLUE);

	////�� ������Ʈ Draw
	//m_pGrid->Draw();

	/////m_pRCube->Draw();
	////m_pRubiksCube->Draw();
	//
	////�� ������Ʈ Draw ��

	////����̽� ȭ�� ���
	//m_pDevice->Flip();
}

// 
void C3DScene::Release()
{

}


//End of C3DScene Functions

///////////////////SceneManager///////////////////////

C3DSceneManager::C3DSceneManager()
{

}
