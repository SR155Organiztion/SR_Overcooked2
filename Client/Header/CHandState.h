#pragma once
#include "CState.h"
#include "Player_Define.h"
#include "CManagement.h"

class CLeftHandIdle : public CState
{
public:
	void Enter_State(CGameObject* Owner) override;
	void Update_State(CGameObject* Owner, const _float& fTimeDelta) override;
	void TestForExit_State(CGameObject* Owner) override;
};

class CRightHandIdle : public CState
{
	void Enter_State(CGameObject* Owner) override;
	void Update_State(CGameObject* Owner, const _float& fTimeDelta) override;
	void TestForExit_State(CGameObject* Owner) override;
};

class CLeftHandWash : public CState
{
	void Enter_State(CGameObject* Owner) override;
	void Update_State(CGameObject* Owner, const _float& fTimeDelta) override;
	void TestForExit_State(CGameObject* Owner) override;
};

class CRightHandWash : public CState
{
	void Enter_State(CGameObject* Owner) override;
	void Update_State(CGameObject* Owner, const _float& fTimeDelta) override;
	void TestForExit_State(CGameObject* Owner) override;
	void Redefine_matrix(CGameObject* Owner);
};

class CLeftHandGrab : public CState
{
	void Enter_State(CGameObject* Owner) override;
	void Update_State(CGameObject* Owner, const _float& fTimeDelta) override;
	void TestForExit_State(CGameObject* Owner) override;
};

class CRightHandGrab : public CState
{
	void Enter_State(CGameObject* Owner) override;
	void Update_State(CGameObject* Owner, const _float& fTimeDelta) override;
	void TestForExit_State(CGameObject* Owner) override;
};

class CLeftHandThrow : public CState
{
	void Enter_State(CGameObject* Owner) override;
	void Update_State(CGameObject* Owner, const _float& fTimeDelta) override;
	void TestForExit_State(CGameObject* Owner) override;
	_float	m_fThrowTime;
	_bool	m_bReturn;
	_bool	m_bEnd;
};

class CRightHandThrow : public CState
{
	void Enter_State(CGameObject* Owner) override;
	void Update_State(CGameObject* Owner, const _float& fTimeDelta) override;
	void TestForExit_State(CGameObject* Owner) override;
	_float	m_fThrowTime;
	_bool	m_bReturn;
	_bool	m_bEnd;
};


class CLeftHandChop : public CState
{
	void Enter_State(CGameObject* Owner) override;
	void Update_State(CGameObject* Owner, const _float& fTimeDelta) override;
	void TestForExit_State(CGameObject* Owner) override;
};

class CRightHandChop : public CState
{
	void Enter_State(CGameObject* Owner) override;
	void Update_State(CGameObject* Owner, const _float& fTimeDelta) override;
	void TestForExit_State(CGameObject* Owner) override;
	_float m_fAngle;
	_bool  m_bCw;
	_float m_fSpeed = 15.f;
};


