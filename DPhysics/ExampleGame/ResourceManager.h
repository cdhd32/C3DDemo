#pragma once
#include"EngineInterface.h" 
#include<unordered_map>
class ResourceManager
{
private:
	HEngine_DX12_3D* m_pHEngine;

	std::unordered_map<std::string, HModelData*> m_models;
	std::unordered_map<std::string, HMaterialData*> m_materials;

public:
	void Initialize(HEngine_DX12_3D* pHEngine);

	HModelData* GetModelPointer(std::string modelName) {return m_models[modelName];}
	HMaterialData* GetMaterialPointer(std::string materialName) { return m_materials[materialName]; }

	void Finalize();
	void DeleteAll();

private:
	void LoadModel(std::string modelName, std::vector<std::string> fbxfiles);
	void LoadMaterial(std::string materialName,
		const WCHAR* albedo, const WCHAR* roughness, const WCHAR* metallic, const WCHAR* ao, const WCHAR* normal, const WCHAR* height);
};
