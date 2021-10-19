#pragma once

//PhysicsEngine 관련 헤더 정리?
//인라인 함수, 매크로 정의도 여기서
#include "PhysX.h"

#include "SimpleMath.h"

#include "PhysicsEngineMain.h"
#include "ISimulationEventCallback.h"

namespace PhysicsEngine
{
	using namespace DirectX::SimpleMath;

	class Rigidbody;
	class Shape;
	class BoxShape;
	class SphereShape;
	class CapsuleShape;
	class TriangleMeshShape;

	class CharacterController;

	class QueryRay;
	class QueryOverlap;

	struct CharaterControllerHit;
	struct RaycastHit;
	struct TriangleMeshData;

	typedef PxControllerCollisionFlags CCTCollisionDirFlags;
	typedef PxControllerCollisionFlag CCTCollisionDirFlag;

	//typedef unsigned int FilterGroupFlag;

	//충돌 검사 결과 저장하는 구조체
	struct CharaterControllerHit
	{
		ISimulationEventCallback* controller;	//!< Current controller
		ISimulationEventCallback* otherController; // Other controller
		Vector3			worldPos;			//!< Contact position in world space
		Vector3			worldNormal;		//!< Contact normal in world space
		Vector3			dir;				//!< Motion direction
		float			length;				//!< Motion length
	};

	struct RaycastHit
	{
		ISimulationEventCallback* object;
		Vector3			worldPos;
		Vector3			worldNormal;
		float			distance;
	};
	
	//#checkpoint
	struct OverlapHit
	{
		ISimulationEventCallback* object;
		//Vector3			worldPos;
		//Vector3			worldNormal;
		//float			distance;
	};

	//OverlapHit g_OverlapHitBuffer[256];

	struct TriangleMeshData
	{
		PxU32 vertNum;
		PxU32 faceNum;
		const float* verts;
		const PxU32* faces;
	};

	typedef PxFilterData FilterGroup;
	typedef PxFilterData FilterMask;

	struct FilterGroupFlag
	{
		enum Enum
		{

		};
	};

	//PX_FLAGS_OPERATORS 메크로로 PxFlags 연산자 오버로딩 그대로 사용
	//
	typedef PxFlags<PhysicsEngine::FilterGroupFlag::Enum, unsigned int> FilterGroupFlags;
	PX_FLAGS_OPERATORS(PhysicsEngine::FilterGroupFlag::Enum, unsigned int)

	//임시 밀도(density) 상수
	const PxReal DEFAULT_DENSITY = 10.0f;

	//convert struct between physx and dx simplemath
	_inline Vector3 ToVector3(const PxVec3& vec3)
	{
		return Vector3(vec3.x, vec3.y, vec3.z);
	};

	_inline Vector2 ToVector2(const PxVec2& vec2)
	{
		return Vector2(vec2.x, vec2.y);
	};

	_inline Quaternion ToQuaternion(const PxQuat& quat)
	{
		return Quaternion(quat.x, quat.y, quat.z, quat.w);
	};

	_inline PxVec3 ToPxVec3(const Vector3& vec3)
	{
		return PxVec3(vec3.x, vec3.y, vec3.z);
	};

	_inline PxVec2 ToPxVec2(const Vector2& vec2)
	{
		return PxVec2(vec2.x, vec2.y);
	};

	_inline PxQuat ToPxQuat(const Quaternion& quat)
	{
		return PxQuat(quat.x, quat.y, quat.z, quat.w);
	};

	_inline PxTransform ToPxTransform(const Vector3& p, const Quaternion& q)
	{
		return PxTransform(ToPxVec3(p), ToPxQuat(q));
	};

	_inline PxTransform ToPxTransform(const Vector3& p)
	{
		return PxTransform(ToPxVec3(p));
	};

	_inline PxPlane ToPxPlane(const Plane& plane)
	{
		return PxPlane(plane.x, plane.y, plane.z, plane.w);
	};

	_inline PxPlane ToPxPlane(const Vector3& normal, const float distance)
	{
		return PxPlane(ToPxVec3(normal), distance);
	};

	_inline PxMat44 ToPxMat44(const Matrix& mat)
	{
		return PxMat44( (float*)mat.m );
	};

	_inline PxMat44 EulerToMat33(const PxVec3& e)
	{
		const float c1 = cosf(e.z);
		const float s1 = sinf(e.z);
		const float c2 = cosf(e.y);
		const float s2 = sinf(e.y);
		const float c3 = cosf(e.x);
		const float s3 = sinf(e.x);
		PxMat44 m;
		m.column0 = PxVec4(c1 * c2, -s1 * c2, s2, 0.0f);
		m.column1 = PxVec4((s1 * c3) + (c1 * s2 * s3), (c1 * c3) - (s1 * s2 * s3), -c2 * s3, 0.0f);
		m.column2 = PxVec4((s1 * s3) - (c1 * s2 * c3), (c1 * s3) + (s1 * s2 * c3), c2 * c3, 0.0f);
		m.column3 = PxVec4(0, 0, 0, 1);
		return m;
	}

	_inline PxVec3 Mat33ToEuler(const PxMat44& m)
	{
		const PxF32 epsilon = 0.99999f;
		PxVec3 e, x, y, z;

		x = PxVec3(m.column0.x, m.column1.x, m.column2.x);
		y = PxVec3(m.column0.y, m.column1.y, m.column2.y);
		z = PxVec3(m.column0.z, m.column1.z, m.column2.z);

		if (x.z > epsilon)
		{
			e.x = PxAtan2(z.y, y.y);
			e.y = PxPi * 0.5f;
			e.z = 0;
		}
		else if (x.z < -epsilon)
		{
			e.x = PxAtan2(z.y, y.y);
			e.y = -PxPi * 0.5f;
			e.z = 0;
		}
		else
		{
			e.x = PxAtan2(-y.z, z.z);
			e.y = PxAsin(x.z);
			e.z = PxAtan2(-x.y, x.x);
		}
		return e;
	}

	_inline void QuaternionToEuler(const Quaternion& quat /*in*/, Vector3& angle /*out*/)
	{
		float sqw = quat.w * quat.w;
		float sqx = quat.x * quat.x;
		float sqy = quat.y * quat.y;
		float sqz = quat.z * quat.z;

		float magnitude = quat.Length();

		double test = quat.x * quat.y + quat.z * quat.w;
		if (test > 0.499 * magnitude) { // singularity at north pole
			angle.y = 2 * atan2(quat.x, quat.w);
			angle.x = DirectX::XM_PIDIV2;
			angle.z = 0;
			return;
		}
		if (test < -0.499 * magnitude) { // singularity at south pole
			angle.y = -2 * atan2(quat.x, quat.w);
			angle.x = -DirectX::XM_PIDIV2;
			angle.z = 0;
			return;
		}

		angle.y = atan2f(2 * quat.y * quat.w - 2 * quat.x * quat.z, sqx - sqy - sqz + sqw);
		angle.x = asinf(2 * test / magnitude);
		angle.z = atan2f(2 * quat.x * quat.w - 2 * quat.y * quat.z, -sqx + sqy - sqz + sqw);
	}

	_inline void EulerToQuaternion(const Vector3& angle /*in*/, Quaternion& quat/*out*/)
	{
		double c1 = cosf(angle.y / 2);
		double s1 = sinf(angle.y / 2);
		double c2 = cosf(angle.x / 2);
		double s2 = sinf(angle.x / 2);
		double c3 = cosf(angle.z / 2);
		double s3 = sinf(angle.z / 2);
		double c1c2 = c1 * c2;
		double s1s2 = s1 * s2;

		quat.w = c1c2 * c3 - s1s2 * s3;
		quat.x = c1c2 * s3 + s1s2 * c3;
		quat.y = s1 * c2 * c3 + c1 * s2 * s3;
		quat.z = c1 * s2 * c3 - s1 * c2 * s3;
	}
};

