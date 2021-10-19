#pragma once
#include"Object.h"

class UnitObject : public virtual Object
{
protected:
	float m_hp;
	float m_mana;
	float m_speed = 1;
	float m_gravity = 10;
	float m_defence;

	XMFLOAT3 m_curruntfront;
	XMFLOAT3 m_destfront;



public:
	UnitObject() :Object() {};
	virtual ~UnitObject() {};

	//virtual void Attack() = 0;
	virtual void Move(float dTime) = 0;
	void RotateByFrontVector(float rotateSpeed);
	//virtual void Attack() = 0;



};
