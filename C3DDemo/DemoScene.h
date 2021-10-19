#pragma once
#pragma comment(lib,"DPhysics.lib")

#include "../C3DEngine/System/C3DScene.h"
#include "../C3DEngine/System/C3DTimer.h"

#include "PhysicsEngine.h"
#include "PhysicsObjects.h"

#include "DemoBox.h"
#include "DemoPlane.h"
#include "DemoPCBox.h"

class DemoScene : public C3DScene
{
protected:
	//test�� ������Ʈ
	C3DDevice* m_pDevice;
	C3DShader* m_pShader; //���߿� ���׸��� Ŭ������ ���̴� ��ü�� ���� #���� //1���̴� ��ü�� 1���̴� ���ҽ� ����
	C3DShader* m_pLineShader;

	C3DCamera* m_pCam;
	C3DLight* m_pDirLight;
	C3DMaterial* m_pMtrl;

	//test�� �׸���
	C3DGrid* m_pGrid;

	//C3DObject* m_pObject;
	//C3DObject** m_pObjArr;

	//cube
	/*RCube* m_pRCube;
	RubiksCube* m_pRubiksCube;*/

	//DemoObjects
	DemoBox* m_Box;
	std::vector<DemoBox*> m_DemoBoxList;
	DemoPCBox* m_PCBox;
	DemoPlane* m_Plane;

	PhysicsEngine::PhysicsEngineMain* m_pPhysicsEngine;

public:
	DemoScene();
	~DemoScene();

	void Initialize(std::wstring sceneName, HWND hWnd);
	void FixedUpdate();
	void Update();
	void Release();
	void Render();

protected:
	//�ſ��� ����� ��ü ����, �ʱ�ȭ
	void _CreateObjects(HWND hWnd);
	//�밳�� ���� ���ۺκп��� �ε������� �ſ����� ����ϴ� ���ҽ��� �ſ��� �ε�
	void _LoadData();
};

