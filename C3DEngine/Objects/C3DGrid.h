#pragma once

class C3DShader;
struct LINE;
struct VB_POS;
class C3DTransform;

//디버그용 그리드 클래스
class C3DGrid
{
public:

private:
	Vector3 m_CentorPos;

	C3DTransform* m_pTransform;

	int m_WidthLineNum;
	int m_HeightLineNum;
	float m_IntervalW;
	float m_IntervalH;

	//Grid를 구성하는 line list 동시에 Grid의 버텍스 정보를 저장하는 버퍼이기도 함
	LINE* m_pLineList;
	VB_POS* m_pTempVB;
	ID3D11Buffer* m_pVB;

	C3DShader* m_pShader; //외부에서 생성한 쉐이더 객체를 사용,
						//외부에서 생성되는 객체랑 내부에서 생성되는 객체랑
						//엄밀히 구분을 하면 좋을 듯, 스마트 포인터? #수정

public:
	//C3DGrid(C3DShader* pShader);
	C3DGrid(C3DShader* pShader, Vector3 centor, int wLineNum, int hLineNum, float intervalW, float intervalH);
	~C3DGrid();

	void Create();
	void Update(float dTime);
	void Draw();
private:
	void _DataLoading();
};

