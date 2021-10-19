#pragma once

class C3DShader;

//DXTK�� SimpleMath���� ���ǵ� �ڷ���(struct)�� ����ؼ� TRS�� ����
//��ü�� TRS�� ���ϴ� ���(Component?)

//2020-08-20
//���� Transform������ Vector3�� ���� �������� �ʴ´�. Set�� Vector3 �� Get�� Matrix�� ����
//TRS ���� Vector�� get�� �� ���� �ְ� ���� ����

//�������� ������ T->R->S ���� ���Ͱ� �ڿ� �������Ƿ� S*R*T ������ TM�� �����.
//�θ� �ִ� ��� 
class C3DTransform
{
	/*enum GizmoMode
	{
		POSITION,
		ROTATION,
		SCALE
	};*/

public:

private:
	//�θ� Transform �׻� TRS �տ� DotProduct �Ѵ�, null�̸�
	C3DTransform* m_pParent;

	Matrix m_TransMatrix;	//�̵� ���
	Matrix m_RotMatrix;	//ȸ�� ���
	Matrix m_ScaleMatrix;	//ũ�� ���
	Matrix m_TransformMatrix;	//T*R*S ��ȯ ���

public:
	C3DTransform();
	C3DTransform(Vector3 pos, Vector3 rot, Vector3 scale);
	C3DTransform(const C3DTransform& trans);
	C3DTransform(Vector3 pos, Vector3 rot, Vector3 scale, C3DTransform* pParents);
	~C3DTransform();

	void Identity();

	Vector3 GetPosition();

	//�θ� Transform Set
	void SetParentTransform(C3DTransform* pParent);
	//Vector3 GetRotation(); //�̱���
	//Vector3 GetScale();	//�̱���
	void SetPosition(Vector3 pos);
	void SetRotation(Vector3 rot);
	void SetRotation(Quaternion qRot);
	void SetScale(Vector3 scale);

	Matrix GetTransMatrix() const { return m_TransMatrix; }
	Matrix GetRotMatrix() const { return m_RotMatrix; }
	Matrix GetScaleMatrix() const { return m_ScaleMatrix; }
	Matrix GetTransformMatrix() const { return m_TransformMatrix; }

	//TRS ���� Ȯ�ο� ����� Draw
	//virtual void RenderGizmo(GizmoMode mode);
	
		

private:

	
};

