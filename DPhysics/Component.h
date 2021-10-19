#pragma once

//컴포넌트 모음 헤더
//필요한 기능에 해당하는 컴포넌트를 멤버(포인터 형)으로 선언 후, 초기화 과정에서 생성(new)를 해주면 됩니다.
//삭제시에는 생성한 컴포넌트별로 Finalize 호출 후 삭제 해주시면 됩니다.
//각 컴포넌트 별로 종속성이 있는 경우가 있으므로 생성/초기화 순서는 Transform - 충돌체 - 강체/CCT 순으로 해주시면 됩니다.
//나중에 object에서 오버라이드 해서 사용하는 가상함수가 필요한 경우에는 클래스를 추가할 예정 #수정

#include "SimpleMath.h"

#include "PhysX.h"
#include "PhysicsEngine.h"

using namespace DirectX::SimpleMath;
using namespace physx;

//변환
#include "Transform.h"

//도형
//#include "TriangleMeshShape.h"

//충돌체
#include "Collider.h"
#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "ShpereCollider.h"
#include "PlaneCollider.h"
#include "TriangleMeshCollider.h"
#include "QuadCollider.h"

//강체
#include "Rigidbody.h"

//캐릭터 컨트롤러 (CCT)
#include "CharacterController.h"

