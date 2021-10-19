#pragma once
#include<Windows.h>
#include<DirectXMath.h>
#include<string>
#include<vector>
#include"Camera.h"
#include"GamePad.h"
#include"Keyboard.h"
#include"Mouse.h"


using namespace DirectX;

typedef LONG_PTR MODELHANDLE;
typedef LONG_PTR MATERIALHANDLE;
typedef LONG_PTR INSTANCEHANDLE;

typedef LONG_PTR DEBUGRECTHANDLE;

struct DebugRect
{
	XMFLOAT3 fourDots[4];
	XMFLOAT4 color;
};

//디버그용 string
struct DebugString
{
	std::string message;
	float r, g, b;
	int posX, posY;
};

enum ShaderType
{
	eNoOption_EI = 0B00000000,
	eTesselation_EI = 0B00000001,
	eMaterialTransform_EI = 0B00000010,

};

namespace EngineInterface
{
	//----------------------------------------------------------------------------
	/* Init & Loop & Terminate Functions.
	*/  
	//----------------------------------------------------------------------------

	/**
	 * Initialize 3D Engine.
	 *
	 * \param hInstance 
	 * \param clientWidth
	 * \param clientHeight
	 */
	void InitEngine(HINSTANCE hInstance, int clientWidth, int clientHeight);

	/**
	 * Loop.
	 * 
	 * This function includes all update of CPU and GPU resources and draw calls.
	 */
	void Loop();
	
	/**
	 * Terminate 3D Engine.
	 * 
	 * This function release all resources has been made.
	 * you need to call this function before termination of your app for proper
	 * memory release.
	 */
	void EndEngine();

	//----------------------------------------------------------------------------
	/* Start & Finishing Engine setting.
	* When you try to make objects which are usually made during initializtion like
	* model or material, you need to call this function before & after.
	* In engine, there is a object called 'resource upload batch', which saves commands
	* to make gpu resources. It saves all the commands between StartSetting() and FinishSetting()
	* submits the commands to gpu at FinishSetting().
	*/
	//----------------------------------------------------------------------------

	/**
	 * Get ready for resource creation.
	 *
	 * Reset 'resource upload batch for new record of commands.
	 */
	void StartSetting();
	
	/**
	 * Excute resource creation.
	 *
	 * Submit commands to gpu for resource creation.
	 */
	void FinishSetting();

	//----------------------------------------------------------------------------
	/* Resource Loading Functions.
	*/  
	//----------------------------------------------------------------------------

	/**
	 * Load model data & animation datas from fbx files. 
	 * 
	 * This function includes both Loading model data and animations.
	 * The string in filePath[0] is file path for model data itself. The Engine only read mesh and bone datas and
	 * skip reading animations.
	 * 
	 * This string in filePath[1,2,3 ...] is file path for animation data. The Engine only reads animation datas
	 * and skip reading mosh and bone datas.
	 * 
	 * You should at least submit one string of file path unless it will throw error.
	 * 
	 * You should keep the returned handle to release model resources.
	 *
	 * \param filePaths filePath of a model.fbx and animations.
	 * 
	 * \return MODELHANDLE a model handle which is used to make instances of it.
	 */
	MODELHANDLE	LoadModel(std::vector<std::string> filePaths);

	/**
	 * Release Model.
	 *
	 * release resources of the model. If a model is released, all the instances of the model are deleted as well. 
	 *  
	 * \param hModel model handle
	 */
	void DeleteModel(MODELHANDLE hModel);

	/**
	 * Load textrue resource and make a material set.
	 *
	 * This function creates a material set from dds files.
	 * If you want to draw Instances with tesselation, you need to prepare 'height map'. otherwise you can skip
	 * 'height map' and just leave it nullptr.
	 * 
	 * \param albedo texture dds file path
	 * \param roughness texture dds file path
	 * \param metallic texture dds file path
	 * \param ao texture dds file path
	 * \param normal texture dds file path
	 * \param height texture dds file path
	 * \return material handle
	 */
	MATERIALHANDLE LoadMaterial(const WCHAR* albedo,
								const WCHAR* roughness,
								const WCHAR* metallic,
								const WCHAR* ao,
								const WCHAR* normal,
								const WCHAR* height = nullptr);
	
	/**
	 * Delete a material set and release resources.
	 *
	 * \param hMaterial material handle
	 */
	void DeleteMaterial(MATERIALHANDLE hMaterial);

	//----------------------------------------------------------------------------
	/* Instance Manage Funtions.
	*/  
	//----------------------------------------------------------------------------

    /** Add instance of a model.
	 * 
	 * This function add a Instance based on a model and materials.
	 * You don't necessarily have to manage 'INSTANCEHANDLE' which is the return of this function unless you want to control the instance.
	 * If you want to control the instance, save the INSTANCEHANDLE to a variable.
	 * 
	 * All the instances will be deleted when the model which the instances are based on delelted by DeleteModel().
	 * 
	 * \param hModel        model handle.
	 * \param shaderFlag    ShaderType, you can use '|'(OR bit operator) to choose mutiple options.
	 * \param worldTM	   initial world transform matrix	.
	 * \param hMat0		   material handle 0.
	 * \param hMat1		   material handle 1. This is used when the instance's shaderFlag has eMaterialTransform_EI. 
	 */
	INSTANCEHANDLE AddInstance(MODELHANDLE hModel, unsigned char shaderFlag, XMFLOAT4X4 worldTM, MATERIALHANDLE hMat0, MATERIALHANDLE hMat1 = 0);

	/**
	 * Delete Instance.
	 *
	 * \param hInst instance handle
	 */
	void DeleteInstance(INSTANCEHANDLE hInst);

	/**
	 * Set wolrd transformation matrix of the instance.
	 *
	 * \param hInst instance handle
	 * \param worldTM world transform matrix
	 */
	void SetInstanceWorldTM(INSTANCEHANDLE hInst, DirectX::SimpleMath::Matrix worldTM);	

	/**
     * Set material handle of the instnace.
     *
     * \param hInst instance handle
     * \param hMat material handle
     * \param slot index 0 or 1(index 1 is used for eMaterialTransform_EI flag instance)
     */
	void SetInstanceMat(INSTANCEHANDLE hInst, MATERIALHANDLE hMat, UINT slot = 0);

	/**
	 * Set animation name of the instance.
	 *
	 * \param hInst			 instance handle
	 * \param animation		 animation name	
	 */
	void SetInstanceAnim(INSTANCEHANDLE hInst, std::string animation);

	/** 
	 * Set animation time of the instance.
	 * 
	 * \param hInst		     instance handle
	 * \param animationTime  animation time
	 */
	void SetInstanceAnimTime(INSTANCEHANDLE hInst, float animationTime);

	/** 
	 * Set lerp value between two materials.
	 * This function is used only if the instance's ShaderType contains eMaterialTransform_EI;
	 * 
	 * \param hInst		         instance handle
	 * \param materialLerpValue  materialLerpValue
	 */
	void SetInstanceMatLerpValue(INSTANCEHANDLE hInst, float materialLerpValue);

	//----------------------------------------------------------------------------
	/* Util functions.
	*/  
	//----------------------------------------------------------------------------

	float GetElapsedTime();
	void SetCamera(Camera camera);
	Camera& GetCamera();
	DirectX::Keyboard* GetKeyboard();
	DirectX::GamePad* GetGamePad();
	DirectX::Mouse* GetMouse();
	DEBUGRECTHANDLE AddDebugRect(DebugRect debugRect);
	void DeleteDebugRect(DEBUGRECTHANDLE key);
	void UpdateDebugRect(DEBUGRECTHANDLE key, DebugRect rect);
	void AddDebugString(DebugString debugString);
}