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
		_bool applyGravity	= true;
		_bool applyRolling	= false;
		_bool applyBouncing = false;
		_bool applyKnockBack = false;
		BOUNDING_TYPE boundingType = BOX;
		COLLISION_OPT collisionOpt = AABB;
	};

protected:
	PHYSICS_OPT	m_stOpt;
	_vec3		m_vBoxMin;
	_vec3		m_vBoxMax;
	_vec3		m_vSphereCenter;
	_float		m_vSphereRadius;

public:
	_vec3* Get_BoxMin() {
		return &m_vBoxMin;
	}

	_vec3* Get_BoxMax() {
		return &m_vBoxMax;
	}

	_vec3* Get_SphereCenter() {
		return &m_vSphereCenter;
	}

	_float* Get_SphereRadius() {
		return &m_vSphereRadius;
	}

	PHYSICS_OPT* Get_Opt() {
		return &m_stOpt;
	}
};