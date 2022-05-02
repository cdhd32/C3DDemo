#include "DemoScene.h"

#include "../C3DEngine/Graphics/C3DShader.h"
#include "../C3DEngine/Objects/C3DMaterial.h"
#include "../C3DEngine/System/C3DTransform.h"
#include "../C3DEngine/Objects/C3DObject.h"

DemoScene::DemoScene()
	:C3DScene()
{

}

DemoScene::~DemoScene()
{

}

void DemoScene::Initialize(std::wstring sceneName, HWND hWnd, C3DDevice* pDevice)
{
	m_SceneName = sceneName;

	_CreateObjects(hWnd, pDevice);
	_LoadData();
	m_pSpriteBatch = new SpriteBatch(pDevice->GetDXDC());
	m_pFont = new SpriteFont(pDevice->GetDevice(), L"../Data/SegoeUI_24.spritefont");

	m_ModelLoader = new C3DModelLoader();
	m_Model = m_ModelLoader->LoadFBX("../Data/PurpleHeart.fbx");
	m_Model->Initialze(m_pShader);

	C3DTimer::GetInstanse()->GetDeltaTime();
}

void DemoScene::FixedUpdate(float deltaTime)
{
	//���� ��ü ������Ʈ
	m_Box->FixedUpdate(deltaTime);
	m_PCBox->FixedUpdate(deltaTime); //PC ������Ʈ
}

void DemoScene::Update()
{
	//�� ������Ʈ �� dTime�� �����´�.
	float dTime = C3DTimer::GetInstanse()->GetDeltaTime();

	//�� ������Ʈ ������Ʈ
	//ObjUpdate(dTime);
	m_pCam->UpdateCamera(dTime);
	m_pDirLight->Update();
	//m_pMtrl->Update();

	//�ӽ� ������� ������Ʈ, cam, light, material �� ��� ���� ���� 
	m_pShader->UpdateTempCB();

	//������� ������Ʈ(GPU)
	m_pShader->UpdateConstantBuffer();


	//�׸��� ������Ʈ
	m_pGrid->Update(dTime);


	//�� ������Ʈ ������Ʈ ��

}

void DemoScene::Release()
{

}

void DemoScene::Render()
{
	//�� ������Ʈ Draw
	m_pGrid->Draw();

	m_Box->RenderUpdate();

	for (int i = 0; i < 1000; i++)
	{
		m_DemoBoxList[i]->RenderUpdate();
	}

	m_Model->RenderUpdate();
	m_PCBox->RenderUpdate();
	//�� ������Ʈ Draw ��

	//��Ʈ ��� ������ ǥ��
	m_pSpriteBatch->Begin();
	m_pFont->DrawString(m_pSpriteBatch, L"Hello, world!", XMFLOAT2(200, 200));
	m_pSpriteBatch->End();

}

void DemoScene::_CreateObjects(HWND hWnd, C3DDevice* pDevice)
{
	//���� ����
	//m_pPhysicsEngine = &PhysicsEngine::PhysicsEngineMain::GetInstance();
	//m_pPhysicsEngine->Initialize();

	//���̴� ����, Device�� DirectXDC�� �������̽��� ��´�
	m_pShader = new C3DShader(pDevice);
	m_pShader->ShaderSetup(L"../Data/fx/Default.fx", SHADERTYPE::SHADER_DEFALT);

	m_pLineShader = new C3DShader(pDevice);
	m_pLineShader->ShaderSetup(L"../Data/fx/Default_WireFrm.fx", SHADERTYPE::SHADER_LINE);

	//Camera 
	m_pCam = new C3DCamera(
		Vector3(0.0f, 50.0f, -100.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.f, 1.f, 0.f),
		60.f,
		(float)g_Mode.Width / (float)g_Mode.Height,
		1.0f, 1000.0f
	);

	//Light
	m_pDirLight = new C3DLight();
	//m_pDirLight->SetUp();

	//Material
	//m_pMtrl = new C3DMaterial();

	//���̴��� ������ۿ� ������ ��ü set
	m_pShader->SetCam(m_pCam);
	m_pShader->SetLight(m_pDirLight);
	//m_pShader->SetMaterial(m_pMtrl);

	m_pLineShader->SetCam(m_pCam);
}

//��, �ؽ���, Scene ���� �� �ε�
void DemoScene::_LoadData()
{
	//Grid
	m_pGrid = new C3DGrid(m_pLineShader, Vector3(0, 0, 0), 21, 21, 10.f, 10.f);
	m_pGrid->Create();

	//DemoBox
	m_Box = new DemoBox(Vector3(10.f, 20.f, 0), Vector3(5.0f, 5.0f, 5.0f), m_pShader);
	m_Box->Initialze();

	//�ڽ� n�� ����
	for (int i = 0; i < 1000; i++)
	{
		//�浹 Ÿ��enum�� �� �ε����� ��ġ�ϰ� �Ѵ�
		//����, ĸ��, �� �� �ϳ��� ���� ���´�
		int colliderType = std::rand() % 3;

		//���� ��ġ, ���׸���, ũ�� ���ڸ� ������ �ٸ� ���� ũ�⳪ ��ġ�� �浹 ���� ��ġ������ ���� #����

		Vector3 pos(-100.f + (float)(std::rand() % 20) * 10.f, 100.f + (float)(std::rand() % 50) * 10.f, -100.f + (float)(std::rand() % 20) * 10.f);
		Vector3 scale((float)(((std::rand() % 5) + 1) * 1),
			(float)(((std::rand() % 5) + 1) * 1),
			(float)(((std::rand() % 5) + 1) * 1));
		DemoBox* pBox = new DemoBox(
			pos,
			scale,
			m_pShader);
		m_DemoBoxList.push_back(pBox);
		pBox->Initialze();
	}

	//DemoPCBox
	m_PCBox = new DemoPCBox(Vector3(-10.f, 20.f, 0), Vector3(10.0f, 10.0f, 10.0f), m_pShader);
	m_PCBox->Initialze();
	//DemoPlane
	m_Plane = new DemoPlane(Vector3(0.f, 1.f, 0.f), 0.f);
	m_Plane->Initialze();
}