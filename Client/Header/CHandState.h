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

class CLeftHandMove : public CState
{
	void Enter_State(CGameObject* Owner) override;
	void Update_State(CGameObject* Owner, const _float& fTimeDelta) override;
	void TestForExit_State(CGameObject* Owner) override;
};

class CRightHandMove : public CState
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
	_float m_fChopTime;
	_float m_fAngle;
	_bool  m_bCw;
	_float m_fSpeed = 10.f;
};


