#pragma once
#include"EngineInterface.h"
#include"PhysicsEngine.h"

#include"ObjectManager.h"
#include"CameraControl.h"
#include<unordered_map>


class ExampleGame
{
private:
	//Devices from HEngine_DX12_3D
	HEngine_DX12_3D* m_p3DgraphicEngine;

	PhysicsEngine::PhysicsEngineMain* m_p3DPhysicsEngine;

	Keyboard* m_keyboard;
	Keyboard::KeyboardStateTracker m_keyboardTracker;

	CameraControl m_cameraControl;

	ObjectManager* m_pObjectManager;

public:
	void Initialize(HINSTANCE hInstance, int clientWidth, int clientHeight);
	void Loop();
	void Finalize();

private:
	HWND InitWindow(HINSTANCE hInstance, int clientWidth, int clientHeight);
	void AddGrid();

};

