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

	_float				m_fSpeed = 10.f;
	_bool				m_bDash = false;
	_float				m_fDashTime;
	PLAYER_ROT			m_eDir;
};

class CPlayerAct : public CState
{
public:
	void		Enter_State(CGameObject* Owner) override;
	void		Update_State(CGameObject* Owner, const _float& fTimeDelta) override;
	void		TestForExit_State(CGameObject* Owner) override;
	ACT_ID		m_eCurAct;
	void		Set_Act(ACT_ID eID) { m_eCurAct = eID; }
};

