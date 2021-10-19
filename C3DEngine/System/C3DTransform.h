#pragma once

class C3DShader;

//DXTK의 SimpleMath에서 정의된 자료형(struct)을 사용해서 TRS를 정의
//객체의 TRS를 정하는 요소(Component?)

//2020-08-20
//이제 Transform에서는 Vector3를 따로 저장하지 않는다. Set은 Vector3 로 Get은 Matrix로 받음
//TRS 별로 Vector를 get을 할 수도 있게 만들 예정

//곱해지는 순서는 T->R->S 지만 벡터가 뒤에 곱해지므로 S*R*T 순서로 TM을 만든다.
//부모가 있는 경우 
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
	//부모 Transform 항상 TRS 앞에 DotProduct 한다, null이면
	C3DTransform* m_pParent;

	Matrix m_TransMatrix;	//이동 행렬
	Matrix m_RotMatrix;	//회전 행렬
	Matrix m_ScaleMatrix;	//크기 행렬
	Matrix m_TransformMatrix;	//T*R*S 변환 행렬

public:
	C3DTransform();
	C3DTransform(Vector3 pos, Vector3 rot, Vector3 scale);
	C3DTransform(const C3DTransform& trans);
	C3DTransform(Vector3 pos, Vector3 rot, Vector3 scale, C3DTransform* pParents);
	~C3DTransform();

	void Identity();

	Vector3 GetPosition();

	//부모 Transform Set
	void SetParentTransform(C3DTransform* pParent);
	//Vector3 GetRotation(); //미구현
	//Vector3 GetScale();	//미구현
	void SetPosition(Vector3 pos);
	void SetRotation(Vector3 rot);
	void SetRotation(Quaternion qRot);
	void SetScale(Vector3 scale);

	Matrix GetTransMatrix() const { return m_TransMatrix; }
	Matrix GetRotMatrix() const { return m_RotMatrix; }
	Matrix GetScaleMatrix() const { return m_ScaleMatrix; }
	Matrix GetTransformMatrix() const { return m_TransformMatrix; }

	//TRS 상태 확인용 기즈모 Draw
	//virtual void RenderGizmo(GizmoMode mode);
	
		

private:

	
};

