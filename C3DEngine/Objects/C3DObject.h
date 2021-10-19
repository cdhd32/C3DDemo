#pragma once

#include <string>
#include <vector>


//�⺻ ������Ʈ Ŭ����
class C3DObject
{
	//��� ����
public:
	//public���� �ַ� ����� ����
	static const C3DObject* NO_PARENT;
protected:
	unsigned int m_id;	//������Ʈ id
	std::wstring m_ObjName;	//�����ڵ� ���� ������Ʈ �̸�

	//���� ���� ������ ���� �ڽ� ������ ����Ʈ, �θ� ������
	//std::vector<C3DObject*> m_ChildObjectList;
	//C3DObject* m_pParent;

	//��� �Լ�
public:
	C3DObject();
	C3DObject(unsigned int id, std::wstring objname);
	C3DObject(const C3DObject& object);

	~C3DObject();

	//
	virtual void Create();
	virtual void Update();

	// ������Ʈ ������ �����ϴ� ���� �ְų� ���� ������ �����ϴ� ���� �ִ� ��� ����
	virtual void Destroy();

	unsigned int GetId() const { return m_id; }
	void SetId(unsigned int val) { m_id = val; }

	std::wstring GetObjName() const;
	void SetObjName(std::wstring val);

	//void SetObjName(TCHAR* strName);

	//���� ���� ���� �ż���
	//void InsertChild(C3DObject* pObject);
	void RemoveChild(C3DObject* pObject);
	void RemoveChild(unsigned int id);

	//void UpdateChild();

protected:

};

//�⺻ ������Ʈ ��ü�� �����ϴ� ��ü, Scene�� �ϳ��� ���
class C3DObjectManager
{
	//��� ����
public:
protected:
	std::vector<C3DObject*> m_ObjectList;
	unsigned int m_ObjectCnt; //������Ʈ�� id�� �ο��ϱ� ���� ī��Ʈ
	//��� �Լ�
public:
	C3DObjectManager();
	~C3DObjectManager();

	//�Ŵ��� �ʱ�ȭ
	//���⼭�� m_ObjectList�� �迭�� �ʿ��� ��ŭ Ȯ���ϴ� ������ ��
	virtual void Initialize(int reqObjectNum = 100); //�⺻ 100�� ������...
	//�Ŵ����� �����ϴ� ��� ������Ʈ�� ������Ʈ
	virtual void UpdateAll();
	//�Ŵ����� �����ϴ� ������Ʈ�� ����
	virtual void ReleaseAll();

	void InsertObject(C3DObject* pObject);
	void DeleteObject(int index);
	void DeleteObject(C3DObject* index);
protected:

};

