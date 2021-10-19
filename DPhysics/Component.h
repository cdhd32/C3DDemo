#pragma once

//������Ʈ ���� ���
//�ʿ��� ��ɿ� �ش��ϴ� ������Ʈ�� ���(������ ��)���� ���� ��, �ʱ�ȭ �������� ����(new)�� ���ָ� �˴ϴ�.
//�����ÿ��� ������ ������Ʈ���� Finalize ȣ�� �� ���� ���ֽø� �˴ϴ�.
//�� ������Ʈ ���� ���Ӽ��� �ִ� ��찡 �����Ƿ� ����/�ʱ�ȭ ������ Transform - �浹ü - ��ü/CCT ������ ���ֽø� �˴ϴ�.
//���߿� object���� �������̵� �ؼ� ����ϴ� �����Լ��� �ʿ��� ��쿡�� Ŭ������ �߰��� ���� #����

#include "SimpleMath.h"

#include "PhysX.h"
#include "PhysicsEngine.h"

using namespace DirectX::SimpleMath;
using namespace physx;

//��ȯ
#include "Transform.h"

//����
//#include "TriangleMeshShape.h"

//�浹ü
#include "Collider.h"
#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "ShpereCollider.h"
#include "PlaneCollider.h"
#include "TriangleMeshCollider.h"
#include "QuadCollider.h"

//��ü
#include "Rigidbody.h"

//ĳ���� ��Ʈ�ѷ� (CCT)
#include "CharacterController.h"

