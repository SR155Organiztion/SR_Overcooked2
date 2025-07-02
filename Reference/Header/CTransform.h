#pragma once
#include "CComponent.h"
#include "IPhysics.h"

BEGIN(Engine)

class ENGINE_DLL CTransform :  public CComponent
{
private:
	explicit CTransform();
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	void		Get_Info(INFO eType, _vec3* pInfo)
	{
		memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
	}

	_vec3		Get_PrevInfo() {
		return m_vPrevPos;
	}

	_vec3		Get_NextInfo() {
		return m_vNextPos;
	}

	void		Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta)
	{
		m_vPrevPos = m_vInfo[INFO_POS];

		m_vVelocity = *pDir * fSpeed;
		m_vInfo[INFO_POS] += m_vVelocity * fTimeDelta;
		m_vNextPos = m_vInfo[INFO_POS] + m_vVelocity * fTimeDelta;
	}

	void Set_Velocity(const _vec3& vVel)
	{
		m_vVelocity = vVel;
	}

	const _vec3& Get_Velocity() const
	{
		return m_vVelocity;
	}

	void		Rotation(ROTATION eType, const _float& fAngle)
	{
		*(((_float*)& m_vAngle) + eType) += fAngle;
	}

	const	_matrix* Get_World() const { return &m_matWorld; }
	void	Get_World(_matrix* pWorld) const
	{
		*pWorld = m_matWorld;
	}
	_vec3				Get_Scale() const { return m_vScale; }

	void	Set_World(const _matrix* pWorld)
	{
		m_matWorld = *pWorld;
	}

	void	Set_Scale(const _vec3& vScale)
	{
		m_vScale = vScale;
	}

	void		Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	const _matrix* Compute_LookAtTarget(const _vec3* pTargetPos);

	void		Set_Pos(_float _fX, _float _fY, _float _fZ)
	{
		m_vInfo[INFO_POS] = { _fX, _fY, _fZ };
	}

	void		Set_Look(const _vec3* _vLook)
	{
		m_vInfo[INFO_LOOK] = *_vLook;
	}

public:
	HRESULT			Ready_Transform();
	virtual _int Update_Component(const _float& fTimeDelta);
	virtual void LateUpdate_Component();

public:
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vScale;
	_vec3			m_vAngle;
	_vec3			m_vPrevPos;
	_vec3			m_vNextPos;
	_vec3			m_vVelocity;

	_matrix			m_matWorld;


public:
	virtual CComponent* Clone();
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free();

};

END
