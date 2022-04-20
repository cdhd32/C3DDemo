#pragma once
#pragma comment(lib,"DPhysics.lib")

#include "../C3DEngine/System/C3DScene.h"
#include "../C3DEngine/System/C3DTimer.h"

#include "../C3DEngine/C3DModel.h"
#include "../C3DEngine/C3DModelLoader.h"

#include "PhysicsEngine.h"
#include "PhysicsObjects.h"

#include "DemoBox.h"
#include "DemoPlane.h"
#include "DemoPCBox.h"

#include "SpriteFont.h"

class DemoScene : public C3DScene
{
protected:
	C3DShader* m_pShader; //나중에 마테리얼 클래스의 쉐이더 객체로 관리 #수정 //1쉐이더 객체당 1쉐이더 리소스 매핑
	C3DShader* m_pLineShader;

	C3DCamera* m_pCam;
	C3DLight* m_pDirLight;
	C3DMaterial* m_pMtrl;

	SpriteBatch* m_pSpriteBatch;
	SpriteFont* m_pFont;

	//test용 그리드
	C3DGrid* m_pGrid;

	//DemoObjects
	DemoBox* m_Box;
	std::vector<DemoBox*> m_DemoBoxList;
	DemoPCBox* m_PCBox;
	DemoPlane* m_Plane;

	C3DModelLoader* m_ModelLoader;
	C3DModel* m_Model;
	//PhysicsEngine::PhysicsEngineMain* m_pPhysicsEngine;

public:
	DemoScene();
	~DemoScene();

	void Initialize(std::wstring sceneName, HWND hWnd, C3DDevice* pDevice);
	void FixedUpdate(float deltaTime);
	void Update();
	void Release();
	void Render();

protected:
	//신에서 사용할 객체 생성, 초기화
	void _CreateObjects(HWND hWnd, C3DDevice* pDevice);
	//대개는 게임 시작부분에서 로딩하지만 신에서만 사용하는 리소스는 신에서 로딩
	void _LoadData();
};

