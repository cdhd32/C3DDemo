#pragma once

class C3DSceneManager;

//main �Ǵ� mainWindow�� ���� ����Ǵ� ���� �ý���, ����
class C3DGame
{
public:
	C3DSceneManager m_SceneManager;
protected:
	
public:
	void Initialize();
	void Update();
	void Release();
protected:
	void LoadGameData();
	void CreateGameObjects();
};

