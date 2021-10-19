#pragma once
#include "DemoBox.h"

//키보드 컨트롤 기능 추가한 박스
//데모에서는 다른 객체들과 충돌하면서 물리엔진 기능이 적용된 것을 보여줌
class DemoPCBox : public DemoBox
{
protected:
	float m_Speed = 10.f;

public:

public:
	DemoPCBox() = default;
	~DemoPCBox() = default;

	DemoPCBox(Vector3 pos,
		Vector3 scale,
		C3DShader* shader)
		: DemoBox(pos, scale, shader)
	{}

	void FixedUpdate(float dTime);

};

