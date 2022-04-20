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
	C3DShader* m_pShader; //���߿� ���׸��� Ŭ������ ���̴� ��ü�� ���� #���� //1���̴� ��ü�� 1���̴� ���ҽ� ����
	C3DShader* m_pLineShader;

	C3DCamera* m_pCam;
	C3DLight* m_pDirLight;
	C3DMaterial* m_pMtrl;

	SpriteBatch* m_pSpriteBatch;
	SpriteFont* m_pFont;

	//test�� �׸���
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
	//�ſ��� ����� ��ü ����, �ʱ�ȭ
	void _CreateObjects(HWND hWnd, C3DDevice* pDevice);
	//�밳�� ���� ���ۺκп��� �ε������� �ſ����� ����ϴ� ���ҽ��� �ſ��� �ε�
	void _LoadData();
};

