#pragma once
#include "Player_Define.h"
#include "CRealPlayer.h"
#include "CGameObject.h"
#include "CState.h"
#include "CTransform.h"

class CPlayerIdle : public CState
{
public:
	void		Enter_State(CGameObject* Owner) override;
	void		Update_State(CGameObject* Owner, const _float& fTimeDelta) override;
	void		TestForExit_State(CGameObject* Owner) override;
};

class CPlayerMove : public CState
{
public:
	void		Enter_State(CGameObject* Owner) override;
	void		Update_State(CGameObject* Owner, const _float& fTimeDelta) override;
	void		TestForExit_State(CGameObject* Owner) override;
	void		Check_Dir(const _float& fTimeDelta);
	_bool		Rotate_Player(CTransform* pTransformCom, const _float& fTimeDelta); /// 플레이어
	void		Move_Player(CTransform* pTransformCom, const _float& fTimeDelta);

	_float				m_fSpeed = 6.f;
	_float				m_fDashTime;
	_float				m_fDashCoolTime = 0.5f;
	_bool				m_bDash, m_bDashCool, m_bCheckKey;
	PLAYER_ROT			m_eDir;
	
	_float				m_fTestEffect;
	_bool				m_bTestEffect;
	CGameObject*		m_pOwner;

};

class CPlayerAct : public CState
{
public:
	void		Enter_State(CGameObject* Owner) override;
	void		Update_State(CGameObject* Owner, const _float& fTimeDelta) override;
	void		TestForExit_State(CGameObject* Owner) override;
	
	void		Set_LookAtStation(CGameObject * Owner, const _float& dt);
	_float		m_fPreAngle, m_fCurAngle;
};

