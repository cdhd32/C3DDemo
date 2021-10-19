#include "../pch.h"

#include "C3DObject.h"

const C3DObject* C3DObject::NO_PARENT = nullptr;

C3DObject::C3DObject()
	: m_id(0), m_ObjName(L"")//, m_pParent(nullptr)
{

}

C3DObject::C3DObject(const C3DObject& object)
	: m_id(object.m_id), m_ObjName(object.m_ObjName)//, m_pParent(object.m_pParent)
{

}

C3DObject::C3DObject(unsigned int id, std::wstring objname)
	:m_id(id), m_ObjName(objname)//, m_pParent(nullptr)
{

}

C3DObject::~C3DObject()
{

}

void C3DObject::Create()
{

}

void C3DObject::Update()
{

}

void C3DObject::Destroy()
{
	//포인터 변수 해제
	/*for (C3DObject* obj : m_ChildObjectList)
	{
		obj->Destroy();
	}*/
	C3DObject::~C3DObject();
}

std::wstring C3DObject::GetObjName() const
{
	return m_ObjName;
}

void C3DObject::SetObjName(std::wstring strName)
{
	m_ObjName = strName;
}

//void C3DObject::InsertChild(C3DObject* pChildObject)
//{
//	pChildObject->m_pParent = this;
//	m_ChildObjectList.push_back(pChildObject);
//}


void C3DObject::RemoveChild(C3DObject* pChildObject)
{
	//일단 for로 검색해봅시다
	/*for (std::vector<C3DObject*>::iterator it = m_ChildObjectList.begin(); it != m_ChildObjectList.end(); ++it)
	{
		if (it == pChildObject)
		{
			m_ChildObjectList.erase(pObject);
		}
	}*/
}

void C3DObject::RemoveChild(unsigned int id)
{
	
}

//void C3DObject::SetObjName(TCHAR* strName)
//{
//	m_ObjName = strName;
//}

////////////////////////////////////////
//C3DObjectManager

C3DObjectManager::C3DObjectManager()
	:m_ObjectCnt(0)
{

}

C3DObjectManager::~C3DObjectManager()
{

}


void C3DObjectManager::Initialize(int reqObjectNum /*= 100*/)
{
	m_ObjectList.reserve(sizeof(C3DObject*) * reqObjectNum);
}

void C3DObjectManager::UpdateAll()
{
	for (C3DObject* obj : m_ObjectList)
	{
		obj->Update();
	}
}

void C3DObjectManager::ReleaseAll()
{
	for (C3DObject* obj : m_ObjectList)
	{
		obj->Destroy();
		obj = nullptr;
	}
}

void C3DObjectManager::InsertObject(C3DObject* pObject)
{
	pObject->SetId(m_ObjectCnt);
	m_ObjectList.push_back(pObject);
	m_ObjectCnt++;
}

//특정 오브젝트를 해제 id로... #수정
void C3DObjectManager::DeleteObject(int index)
{

}

//특정 오브젝트를 해제 object 포인터 비교로? #수정
void C3DObjectManager::DeleteObject(C3DObject* index)
{

}
