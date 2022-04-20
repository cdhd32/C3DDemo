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
	C3DShader* m_pShader; //나중에 마테리얼 클래스의 쉐이더 객체로 관리 #수정 //1쉐이더 객체당 1쉐이더 리소스 매핑
	C3DShader* m_pLineShader;

	C3DCamera* m_pCam;
	C3DLight* m_pDirLight;
	C3DMaterial* m_pMtrl;

	//test용 object
	C3DGrid* m_pGrid;

public:
	C3DRenderer();
	~C3DRenderer();
	int		Initialize(HWND hWnd);
	int		DataLoading();
	
//SceneManager로 따로 빼기 #수정
	void	SceneUpdate();
	void	SceneRender();

	void	DataRelease();
protected:
};