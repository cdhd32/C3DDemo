#pragma once

class C3DShader;
struct LINE;
struct VB_POS;
class C3DTransform;

//����׿� �׸��� Ŭ����
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

	//Grid�� �����ϴ� line list ���ÿ� Grid�� ���ؽ� ������ �����ϴ� �����̱⵵ ��
	LINE* m_pLineList;
	VB_POS* m_pTempVB;
	ID3D11Buffer* m_pVB;

	C3DShader* m_pShader; //�ܺο��� ������ ���̴� ��ü�� ���,
						//�ܺο��� �����Ǵ� ��ü�� ���ο��� �����Ǵ� ��ü��
						//������ ������ �ϸ� ���� ��, ����Ʈ ������? #����

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

