#include "../pch.h"

#include "../Graphics/C3DDevice.h"
#include "../Graphics/C3DShader.h"

#include "../Objects/C3DCamera.h"
#include "../Objects/C3DLight.h"
#include "../Objects/C3DMaterial.h"

#include "../System/C3DTimer.h"

#include "../Objects/C3DGrid.h"
#include "../Objects/C3DObject.h"

#include "C3DScene.h"

C3DScene::C3DScene()
	:m_SceneId(0), m_SceneName(L"")
{

}

C3DScene::~C3DScene()
{

}

void C3DScene::Initialize(std::wstring sceneName, HWND hWnd)
{

}

void C3DScene::FixedUpdate(float deltaTime)
{

}

void C3DScene::Update()
{

}

void C3DScene::Render()
{

}


void C3DScene::Release()
{

}


//End of C3DScene Functions

///////////////////SceneManager///////////////////////

C3DSceneManager::C3DSceneManager()
{

}
