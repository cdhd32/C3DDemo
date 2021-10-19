#include"ResourceManager.h"

void ResourceManager::Initialize(HEngine_DX12_3D* pHEngine)
{
	m_pHEngine = pHEngine;

	m_pHEngine->StartSetting();

	std::vector<std::string> fbxList;
	fbxList.push_back("Media/Model/cylinder.fbx");
	fbxList.push_back("Media/Model/cylinder@idle.fbx");
	fbxList.push_back("Media/Model/cylinder@run.fbx");

	//fbxList.push_back("Media/Model/macho.fbx");
	//fbxList.push_back("Media/Model/macho@run.fbx");

	//fbxList.push_back("Media/Model/Demon@dance.fbx");
	//fbxList.push_back("Media/Model/Demon@dance.fbx");
	//map


	LoadModel("cylinder", fbxList);

	fbxList.clear();
	fbxList.push_back("Media/Model/map_re_re_re.fbx");
	//fbxList.push_back("Media/Model/testMap.fbx");
	

	LoadModel("map", fbxList);

	LoadMaterial("cobble", L"Media/PBR/cobble_albedo.png",
		L"Media/PBR/cobble_roughness.png",
		L"Media/PBR/cobble_metallic.png",
		L"Media/PBR/cobble_ao.png",
		L"Media/PBR/cobble_normal.png",
		L"Media/PBR/cobble_height.png");

	LoadMaterial("bamboo", L"Media/PBR/bamboo_albedo.png",
		L"Media/PBR/bamboo_roughness.png",
		L"Media/PBR/bamboo_metallic.png",
		L"Media/PBR/bamboo_ao.png",
		L"Media/PBR/bamboo_normal.png",
		nullptr);

	LoadMaterial("metal", L"Media/PBR/metal_albedo.png",
		L"Media/PBR/metal_roughness.png",
		L"Media/PBR/metal_metallic.png",
		L"Media/PBR/metal_ao.png",
		L"Media/PBR/metal_normal.png",
		nullptr);

	m_pHEngine->FinishSetting();
}

void ResourceManager::LoadModel(std::string modelName, std::vector<std::string> fbxfiles)
{
	m_models[modelName] = m_pHEngine->CreateModel(fbxfiles);
}

void ResourceManager::LoadMaterial(std::string materialName, const WCHAR* albedo, const WCHAR* roughness, const WCHAR* metallic, const WCHAR* ao, const WCHAR* normal, const WCHAR* height)
{
	m_materials[materialName] = m_pHEngine->CreateMaterial(albedo, roughness, metallic, ao, normal, height);
}

void ResourceManager::Finalize()
{
	DeleteAll();
}

void ResourceManager::DeleteAll()
{
	m_pHEngine->StartSetting();

	for (std::pair<const std::string, HModelData*>& model : m_models)
	{
		model.second->Delete();
	}

	for (std::pair<const std::string, HMaterialData*>& material : m_materials)
	{
		material.second->Delete();
	}

	m_pHEngine->FinishSetting();

	m_models.clear();
	m_materials.clear();
}
