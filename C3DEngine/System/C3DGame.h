#pragma once

class C3DSceneManager;

//main 또는 mainWindow를 통해 실행되는 게임 시스템, 루프
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

