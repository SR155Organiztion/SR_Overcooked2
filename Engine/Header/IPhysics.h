#pragma once

class IPhysics {
public:
	enum BOUNDING_TYPE {
		BOX
		, SPHERE
	};

	enum COLLISION_OPT {
		AABB
		, OBB
	};
private:
	struct PHYSICS_OPT {
		_bool			bApplyGravity	= true;
		_bool			bApplyRolling	= false;
		_bool			bApplyBouncing = false;
		_bool			bApplyKnockBack = false;
		_float			fReflectSpeed = 0.98f;
		_float			fKnockbackScale = 5.f;
		_float			fDeceleration = 0.97f;
		BOUNDING_TYPE	eBoundingType = BOX;
		COLLISION_OPT	stCollisionOpt = AABB;
	};

protected:
	PHYSICS_OPT	m_stOpt;
	_vec3		m_vMinBox;
	_vec3		m_vMaxBox;
	_vec3		m_vNextMinBox;
	_vec3		m_vNextMaxBox;
	_vec3		m_vSphereCenter;
	_vec3		m_vCollisionDir;
	_vec3		m_vReflectionVelocity;
	_float		m_vSphereRadius = 0.f;
	_float		m_fGravityElapsed = 0.f;
	_bool		m_bIsGround = true;
	

public:
	_vec3* Get_MinBox() {
		return &m_vMinBox;
	}

	_vec3* Get_MaxBox() {
		return &m_vMaxBox;
	}

	_vec3* Get_NextMinBox() {
		return &m_vNextMinBox;
	}

	_vec3* Get_NextMaxBox() {
		return &m_vNextMaxBox;
	}

	_vec3* Get_SphereCenter() {
		return &m_vSphereCenter;
	}

	_float* Get_SphereRadius() {
		return &m_vSphereRadius;
	}

	_float* Get_GravityElased() {
		return &m_fGravityElapsed;
	}

	_bool* Get_IsGround() {
		return &m_bIsGround;
	}

	PHYSICS_OPT* Get_Opt() {
		return &m_stOpt;
	}

	_vec3* Get_CollisionDir() {
		return &m_vCollisionDir;
	}

	_vec3* Get_ReflectionVelocity() {
		return &m_vReflectionVelocity;
	}

	void Set_CollisionDir(_vec3* _vCollisionDir) {
		m_vCollisionDir = *_vCollisionDir;
	}

	void Set_ReflectionVelocity(_vec3* _vReflectionVelocity) {
		m_vReflectionVelocity = *_vReflectionVelocity;
	}

	void Set_BoundingBox(
		_vec3 _vMinBox, _vec3 _vMaxBox
		, _vec3 _vNextMinBox, _vec3 _vNextMaxBox
	) {
		m_vMinBox = _vMinBox;
		m_vMaxBox = _vMaxBox;
		m_vNextMinBox = _vNextMinBox;
		m_vNextMaxBox = _vNextMaxBox;
	}

	void Set_GravityElapsed(_float _fGravityElapsed) {
		m_fGravityElapsed = _fGravityElapsed;
	}

	void Set_IsGround(_bool _bIsGround) {
		m_bIsGround = _bIsGround;
	}
};