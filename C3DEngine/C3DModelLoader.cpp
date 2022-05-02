#include "C3DModelLoader.h"

#include "C3DModel.h"
#include "Graphics/C3DShader.h"

/* Tab character ("\t") counter */
int numTabs = 0;

/**
 * Print the required number of tabs.
 */
void PrintTabs() {
    for (int i = 0; i < numTabs; i++)
        printf("\t");
}

/**
* Return a string-based representation based on the attribute type.
*/

FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
    switch (type) {
    case FbxNodeAttribute::eUnknown: return "unidentified";
    case FbxNodeAttribute::eNull: return "null";
    case FbxNodeAttribute::eMarker: return "marker";
    case FbxNodeAttribute::eSkeleton: return "skeleton";
    case FbxNodeAttribute::eMesh: return "mesh";
    case FbxNodeAttribute::eNurbs: return "nurbs";
    case FbxNodeAttribute::ePatch: return "patch";
    case FbxNodeAttribute::eCamera: return "camera";
    case FbxNodeAttribute::eCameraStereo: return "stereo";
    case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
    case FbxNodeAttribute::eLight: return "light";
    case FbxNodeAttribute::eOpticalReference: return "optical reference";
    case FbxNodeAttribute::eOpticalMarker: return "marker";
    case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
    case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
    case FbxNodeAttribute::eBoundary: return "boundary";
    case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
    case FbxNodeAttribute::eShape: return "shape";
    case FbxNodeAttribute::eLODGroup: return "lodgroup";
    case FbxNodeAttribute::eSubDiv: return "subdiv";
    default: return "unknown";
    }
}

/**
    * Print an attribute.
    */
void PrintAttribute(FbxNodeAttribute* pAttribute) {
    if (!pAttribute) return;

    FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
    FbxString attrName = pAttribute->GetName();
    PrintTabs();
    // Note: to retrieve the character array of a FbxString, use its Buffer() method.
    printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

/**
    * Print a node, its attributes, and all its children recursively.
    */
void PrintNode(FbxNode* pNode) {
    PrintTabs();
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get();
    FbxDouble3 rotation = pNode->LclRotation.Get();
    FbxDouble3 scaling = pNode->LclScaling.Get();

    // Print the contents of the node.
    printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
        nodeName,
        translation[0], translation[1], translation[2],
        rotation[0], rotation[1], rotation[2],
        scaling[0], scaling[1], scaling[2]
    );
    numTabs++;

    // Print the node's attributes.
    for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
        PrintAttribute(pNode->GetNodeAttributeByIndex(i));

    // Recursively print the children.
    for (int j = 0; j < pNode->GetChildCount(); j++)
        PrintNode(pNode->GetChild(j));

    numTabs--;
    PrintTabs();
    printf("</node>\n");
}

void LoadVertex(FbxNode* pNode) {
    PrintTabs();
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get();
    FbxDouble3 rotation = pNode->LclRotation.Get();
    FbxDouble3 scaling = pNode->LclScaling.Get();

    //버텍스 로딩
    //FbxMesh* pMesh = pNode->GetMesh();
    //pMesh->get

    // Print the contents of the node.
    printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
        nodeName,
        translation[0], translation[1], translation[2],
        rotation[0], rotation[1], rotation[2],
        scaling[0], scaling[1], scaling[2]
    );
    numTabs++;

    // Print the node's attributes.
    for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
        PrintAttribute(pNode->GetNodeAttributeByIndex(i));

    // Recursively print the children.
    for (int j = 0; j < pNode->GetChildCount(); j++)
        PrintNode(pNode->GetChild(j));

    numTabs--;
    PrintTabs();
    printf("</node>\n");
}

C3DModelLoader::C3DModelLoader()
{
}

C3DModelLoader::~C3DModelLoader()
{
}

C3DModel* C3DModelLoader::LoadFBX(const std::string& fileName)
{
    // Initialize the SDK manager. This object handles memory management.
    FbxManager* lSdkManager = FbxManager::Create();

    // Create the IO settings object.
    FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    // Create an importer using the SDK manager.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

    // Use the first argument as the filename for the importer.
    if (!lImporter->Initialize(fileName.c_str(), -1, lSdkManager->GetIOSettings())) {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);


    }

    // Create a new scene so that it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);

    // The file is imported; so get rid of the importer.
    lImporter->Destroy();

    // Print the nodes of the scene and their attributes recursively.
    // Note that we are not printing the root node because it should
    // not contain any attributes.

    //기존 버텍스 구조체에 입력 작업중 #checkpoint
    int controlPointsCnt = 0;
    int triCnt = 0;
    C3DModel* pModel = new C3DModel;

    FbxNode* lRootNode = lScene->GetRootNode();
    lRootNode = lRootNode->FindChild("PurpleHeartV");
    if (lRootNode->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eMesh)
        assert(true);

    if (lRootNode) {
        int cnt = lRootNode->GetChildCount();
       
        //PrintNode(lRootNode->GetChild(i));
        FbxMesh* pMesh = lRootNode->GetMesh();
        controlPointsCnt = pMesh->GetControlPointsCount();
        triCnt = pMesh->GetPolygonCount();

        pModel->m_pModel = new VERTEX[triCnt];
        pModel->m_VtxNum = triCnt;
        for (int i = 0; i < triCnt; i++)
        {
            pModel->m_pModel[i].x = pMesh->GetControlPointAt(i).mData[0];
            pModel->m_pModel[i].y = pMesh->GetControlPointAt(i).mData[1];
            pModel->m_pModel[i].z = pMesh->GetControlPointAt(i).mData[2];

            FbxGeometryElementNormal* pNormal = pMesh->GetElementNormal(0);
            FbxLayerElement::EMappingMode mappingMode = pNormal->GetMappingMode();
            pModel->m_pModel[i].nx = pNormal->GetDirectArray().GetAt(i).mData[0];
            pModel->m_pModel[i].ny = pNormal->GetDirectArray().GetAt(i).mData[1];
            pModel->m_pModel[i].nz = pNormal->GetDirectArray().GetAt(i).mData[2];
        }
    }
    // Destroy the SDK manager and all the other objects it was handling.
    lSdkManager->Destroy();
	return pModel;
}
