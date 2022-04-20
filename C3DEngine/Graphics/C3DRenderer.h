#pragma once

class C3DDevice;
class C3DShader;
class C3DCamera;
class C3DLight;
class C3DMaterial;

class C3DObject;

class C3DGrid;

class C3DRenderer
{
public:
	/*VERTEX m_vtxCube;
	XMFLOAT3 m_vCubePos;
	XMFLOAT3 m_vCubeRot;
	XMFLOAT3 m_vCubeScale;*/
protected:
	HWND m_hWnd;

	C3DDevice* m_pDevice;
	C3DShader* m_pShader; //���߿� ���׸��� Ŭ������ ���̴� ��ü�� ���� #���� //1���̴� ��ü�� 1���̴� ���ҽ� ����
	C3DShader* m_pLineShader;

	C3DCamera* m_pCam;
	C3DLight* m_pDirLight;
	C3DMaterial* m_pMtrl;

	//test�� object
	C3DGrid* m_pGrid;

public:
	C3DRenderer();
	~C3DRenderer();
	int		Initialize(HWND hWnd);
	int		DataLoading();
	
//SceneManager�� ���� ���� #����
	void	SceneUpdate();
	void	SceneRender();

	void	DataRelease();
protected:
};