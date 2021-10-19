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

// 초기화 함수, Scene 파일 입출력 기능을 넣는다면 m_SceneFileName을 저장하게 확장한다
// scene 파일 이름은 sceneName과 같은 이름으로 저장한다. 
//void C3DScene::Initialize(std::wstring sceneName)
//{
//	m_SceneName = sceneName;
//	//_CreateObjects();
//	//_LaodData();
//}

//hWnd 전달용 scene test 함수
void C3DScene::Initialize(std::wstring sceneName, HWND hWnd)
{
//	m_SceneName = sceneName;
//	
//	_CreateObjects(hWnd);
//	_LoadData();
//
//	//로딩 끝나면 BGM 재생 시작, 주석 제거하면 정상 작동함
//	//m_SoundEngine.PlayBGMSound(eSoundResourceBGM::INGAME);
//	m_SoundEngine.SetBGMVolume(0.8);
}

void C3DScene::FixedUpdate()
{
}

// Manager에서 업데이트하는 함수
void C3DScene::Update()
{
	////매 업데이트 시 dTime을 가져온다.
	//float dTime = C3DTimer::GetInstanse()->GetDeltaTime();
	//
	////test code -> 실제 구현은 상속받은 클래스에서...

	////랜더 상태 업테이트, 여기 보다 다른 곳에서 호출하는 게 좋을 것 같다. #수정
	//m_pDevice->RenderStateUpdate();

	////신 오브젝트 업데이트

	////ObjUpdate(dTime);
	//m_pCam->UpdateCamera(dTime);
	//m_pDirLight->Update();
	////m_pMtrl->Update();

	////임시 상수버퍼 업데이트, cam, light, material 등 상수 버퍼 정보 
	//m_pShader->UpdateTempCB();

	////상수버퍼 업데이트(GPU)
	//m_pShader->UpdateConstantBuffer();

	////오브젝트의 worldTM 만들기 -> 오브젝트 별로 갱신
	///*XMMATRIX mTM = XMMatrixIdentity();
	//XMMATRIX _mScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//XMMATRIX _mRot = XMMatrixRotationRollPitchYaw(0, 0, 0);
	//XMMATRIX _mTrans = XMMatrixTranslation(0, 0, 0);
	//mTM = _mScale * _mRot * _mTrans;*/

	////m_pShader->SetTramsform2();// 상수버퍼의 view, proj 만 갱신 

	////m_pShader->SetTransform(TS_WORLD, mTM);//오브젝트 worldTM 정보만 업데이트

	////그리드 업데이트
	//m_pGrid->Update(dTime);

	////큐브 업데이트
	/////m_pRCube->Update(dTime);
	////m_pRubiksCube->Update(dTime);

	//
	////신 오브젝트 업데이트 끝


}

// Manager에서 Render하는 함수
void C3DScene::Render()
{
	////test code -> 실제 구현은 상속받은 클래스에서...
	////백버퍼 클리어
	//m_pDevice->RTClear(C3DDevice::CLEAR_COLOR_DBLUE);

	////신 오브젝트 Draw
	//m_pGrid->Draw();

	/////m_pRCube->Draw();
	////m_pRubiksCube->Draw();
	//
	////신 오브젝트 Draw 끝

	////디바이스 화면 출력
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
