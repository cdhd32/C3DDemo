#pragma once

class C3DMaterial
{
public:
	static const C3DMaterial DIFFUSE_RED;
	static const C3DMaterial DIFFUSE_BLUE;
	static const C3DMaterial DIFFUSE_GREEN;
	static const C3DMaterial DIFFUSE_YELLOW;
	static const C3DMaterial DIFFUSE_CYAN;
	static const C3DMaterial DIFFUSE_MAGENTA;
	static const C3DMaterial DIFFUSE_WHITE;
	static const C3DMaterial DIFFUSE_BLACK;
	static const C3DMaterial DIFFUSE_GRAY;

	static const C3DMaterial DIFFUSE_ORANGE;

protected:
	Color m_Diffuse;     //확산광 반사율(%) 
	Color m_Ambient;     //주변광 반사율(%) 
	Color m_Specular;    //정반사광 반사율(%)
	FLOAT m_Power;       //정반사 보정값 : "거칠기" 
public:
	C3DMaterial();
	C3DMaterial(Color& diffuse, Color& ambient, Color& specular, float power);
	~C3DMaterial();
	void Create();
	void Update();
	void Release();

	Vector4 GetDiffuse() const { return m_Diffuse; }
	void SetDiffuse(Vector4 val) { m_Diffuse = val; }
	Vector4 GetAmbient() const { return m_Ambient; }
	void SetAmbient(Vector4 val) { m_Ambient = val; }
	Vector4 GetSpecular() const { return m_Specular; }
	void SetSpecular(Vector4 val) { m_Specular = val; }
	FLOAT GetPower() const { return m_Power; }
	void SetPower(FLOAT val) { m_Power = val; }
protected:
};

