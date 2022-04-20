#pragma once

#include <Windows.h>
#include <string>
#include <vector>

//#include "../Sound/SoundEngine.h" //fmod ���� ����

#include "../Graphics/C3DDevice.h"
#include "../Graphics/C3DRenderer.h"
#include "../Graphics/C3DShader.h"

#include "../Objects/C3DCamera.h"
#include "../Objects/C3DLight.h"
#include "../Objects/C3DMaterial.h"

#include "../System/C3DTimer.h"

#include "../Objects/C3DGrid.h"
#include "../Objects/C3DObject.h"

//class C3DDevice;
//class C3DShader;
//class C3DCamera;
//class C3DLight;
//class C3DMaterial;
//
//class C3DGrid;
//class C3DObject;
//class RCube;
//class RubiksCube;
//class CubeSurface;
//
//class SoundEngine;

class C3DScene
{
public:

protected:
	//�� id
	std::wstring m_SceneName;
	unsigned int m_SceneId;
	//��, �������� �������� ��Ÿ ����, �ؽ�Ʈ�� ���� ���� ������ �������� �ϳ���...
	//std::wstring m_SceneFileName;

public:
	C3DScene();
	~C3DScene();

	//void Initialize(std::wstring sceneName);
	void Initialize(std::wstring sceneName, HWND hWnd);
	void FixedUpdate(float deltaTime);
	void Update();
	void Render();
	void Release();
};

class C3DSceneManager
{
public:
	
protected:
	std::vector<C3DScene*> m_SceneList;
	unsigned int m_SceneCnt;
	C3DScene* m_pCurScene;
public:
	C3DSceneManager();
	~C3DSceneManager();

	void LoadScenes();

	void Initialize();
	void Update();
	void Render();
	void Release();

	void SetCurScene(C3DScene* scene) { m_pCurScene = scene; }

protected:
	//�ſ��� ����� ��ü ����
	void _CreateData();
	//�밳�� ���� ���ۺκп��� �ε������� �ſ����� ����ϴ� ���ҽ��� �ſ��� �ε�
	void _LoadData();

	//C3DScene* _FindSceneById(unsigned int id);
	//C3DScene* _FindSceneByName(std::wstring name);
};
