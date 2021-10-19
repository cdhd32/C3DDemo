#pragma once

#include <string>
#include <vector>


//기본 오브젝트 클래스
class C3DObject
{
	//멤버 변수
public:
	//public에서 주로 상수를 정의
	static const C3DObject* NO_PARENT;
protected:
	unsigned int m_id;	//오브젝트 id
	std::wstring m_ObjName;	//유니코드 대응 오브젝트 이름

	//계층 구조 구현을 위한 자식 포인터 리스트, 부모 포인터
	//std::vector<C3DObject*> m_ChildObjectList;
	//C3DObject* m_pParent;

	//멤버 함수
public:
	C3DObject();
	C3DObject(unsigned int id, std::wstring objname);
	C3DObject(const C3DObject& object);

	~C3DObject();

	//
	virtual void Create();
	virtual void Update();

	// 오브젝트 내에서 참조하는 것이 있거나 직접 힙에서 생성하는 것이 있는 경우 선언
	virtual void Destroy();

	unsigned int GetId() const { return m_id; }
	void SetId(unsigned int val) { m_id = val; }

	std::wstring GetObjName() const;
	void SetObjName(std::wstring val);

	//void SetObjName(TCHAR* strName);

	//계층 구조 관련 매서드
	//void InsertChild(C3DObject* pObject);
	void RemoveChild(C3DObject* pObject);
	void RemoveChild(unsigned int id);

	//void UpdateChild();

protected:

};

//기본 오브젝트 객체를 관리하는 객체, Scene당 하나씩 사용
class C3DObjectManager
{
	//멤버 변수
public:
protected:
	std::vector<C3DObject*> m_ObjectList;
	unsigned int m_ObjectCnt; //오브젝트에 id를 부여하기 위한 카운트
	//멤버 함수
public:
	C3DObjectManager();
	~C3DObjectManager();

	//매니저 초기화
	//여기서는 m_ObjectList의 배열을 필요한 만큼 확장하는 동작을 함
	virtual void Initialize(int reqObjectNum = 100); //기본 100개 정도만...
	//매니저과 관리하는 모든 오브젝트를 업데이트
	virtual void UpdateAll();
	//매니저가 관리하는 오브젝트를 해제
	virtual void ReleaseAll();

	void InsertObject(C3DObject* pObject);
	void DeleteObject(int index);
	void DeleteObject(C3DObject* index);
protected:

};

