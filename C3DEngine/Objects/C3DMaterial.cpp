#include "../pch.h"

#include "C3DMaterial.h"

const C3DMaterial C3DMaterial::DIFFUSE_RED = C3DMaterial(	//red
	Color(1.f, 0.f, 0.f, 1.f),
	Color(1.f, 0.f, 0.f, 1.f),
	Color(1.f, 1.f, 1.f, 1.f),
	10.f
);
const C3DMaterial C3DMaterial::DIFFUSE_BLUE = C3DMaterial(	//green
	Color(0.f, 1.f, 0.f, 1.f),
	Color(0.f, 1.f, 0.f, 1.f),
	Color(1.f, 1.f, 1.f, 1.f),
	10.f
);
const C3DMaterial C3DMaterial::DIFFUSE_GREEN = C3DMaterial(	//blue
	Color(0.f, 0.f, 1.f, 1.f),
	Color(0.f, 0.f, 1.f, 1.f),
	Color(1.f, 1.f, 1.f, 1.f),
	10.f
);
const C3DMaterial C3DMaterial::DIFFUSE_YELLOW = C3DMaterial(	//yellow
	Color(1.f, 1.f, 0.f, 1.f),
	Color(1.f, 1.f, 0.f, 1.f),
	Color(1.f, 1.f, 1.f, 1.f),
	10.f
);
const C3DMaterial C3DMaterial::DIFFUSE_CYAN = C3DMaterial(	//Cyan
	Color(0.f, 1.f, 1.f, 1.f),
	Color(0.f, 1.f, 1.f, 1.f),
	Color(1.f, 1.f, 1.f, 1.f),
	10.f
);
const C3DMaterial C3DMaterial::DIFFUSE_MAGENTA = C3DMaterial(	//Magenta
	Color(1.f, 0.f, 1.f, 1.f),
	Color(1.f, 0.f, 1.f, 1.f),
	Color(1.f, 1.f, 1.f, 1.f),
	10.f
);
const C3DMaterial C3DMaterial::DIFFUSE_WHITE = C3DMaterial(	//white
	Color(1.f, 1.f, 1.f, 1.f),
	Color(1.f, 1.f, 1.f, 1.f),
	Color(1.f, 1.f, 1.f, 1.f),
	10.f
);

const C3DMaterial C3DMaterial::DIFFUSE_BLACK = C3DMaterial(	//black(À¯±¤)
	Color(0.f, 0.f, 0.f, 1.f),
	Color(0.f, 0.f, 0.f, 1.f),
	Color(1.f, 1.f, 1.f, 1.f),
	10.f
);

const C3DMaterial C3DMaterial::DIFFUSE_GRAY = C3DMaterial(	//gray
	Color(0.5f, 0.5f, 0.5f, 1.f),
	Color(0.5f, 0.5f, 0.5f, 1.f),
	Color(1.f, 1.f, 1.f, 1.f),
	10.f
);

const C3DMaterial C3DMaterial::DIFFUSE_ORANGE = C3DMaterial(	//gray
	Color(1.0f, 0.647f, 0.0f, 1.f),
	Color(1.0f, 0.647f, 0.0f, 1.f),
	Color(1.f, 1.f, 1.f, 1.f),
	10.f
);

C3DMaterial::C3DMaterial()
	: m_Diffuse(1.f, 0.f, 0.f, 1.f),
	m_Ambient(1.f, 0.f, 0.f, 1.f),
	m_Specular(1.f, 1.f, 1.f, 1.f),
	m_Power(10.f)
{

}

C3DMaterial::C3DMaterial(Color& diffuse, Color& ambient, Color& specular, float power)
	: m_Diffuse(diffuse),
	m_Ambient(ambient),
	m_Specular(specular),
	m_Power(power)
{

}

C3DMaterial::~C3DMaterial()
{

}

void C3DMaterial::Create()
{
	Update();
}

void C3DMaterial::Update()
{
	/*g_Mtrl.Diffuse = XMFLOAT4A(1.f, 0.f, 0.f, 1.f);
	g_Mtrl.Ambient = XMFLOAT4A(1.f, 0.f, 0.f, 1.f);
	g_Mtrl.Specular = XMFLOAT4A(1.f, 1.f, 1.f, 1.f);
	g_Mtrl.Power = 10.f;*/
}
