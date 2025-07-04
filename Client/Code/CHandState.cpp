#include "pch.h"
#include "CHandState.h"
#include "CPlayerHand.h"
#include "CTransform.h"
#include "CDInputMgr.h"
#include "CFSMComponent.h"

//---------------- Player_Idle ----------------//
void CLeftHandIdle::Enter_State(CGameObject* Owner)
{
	//MSG_BOX("Left Hand Enter Idle");

}

void CLeftHandIdle::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{
	CPlayerHand* pHand = dynamic_cast<CPlayerHand*>(Owner);
	REVINFO* pRevInfo = pHand->Get_RevInfo();
	pRevInfo->m_vecRevTrans = { 0.f, 0.f, 0.5f };
	pRevInfo->m_fRevAngleX = (_float)D3DXToRadian(90.f);


}
	
void CLeftHandIdle::TestForExit_State(CGameObject* Owner)
{
	
}

void CRightHandIdle::Enter_State(CGameObject* Owner)
{
	//MSG_BOX("Right Hand Enter Idle");

}

void CRightHandIdle::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{
	CPlayerHand* pHand = dynamic_cast<CPlayerHand*>(Owner);
	REVINFO* pRevInfo = pHand->Get_RevInfo();
	pRevInfo->m_vecRevTrans = { 0.f, 0.f, 0.5f };
	pRevInfo->m_fRevAngleX = (_float)D3DXToRadian(90.f);

}

void CRightHandIdle::TestForExit_State(CGameObject* Owner)
{
}

//---------------- Player_Move ----------------//
void CLeftHandMove::Enter_State(CGameObject* Owner)
{
	//MSG_BOX("Left Hand Enter Move");	
	CPlayerHand* pHand = dynamic_cast<CPlayerHand*>(Owner);
	REVINFO* pRevInfo = pHand->Get_RevInfo();
	pRevInfo->m_vecRevTrans = { 0.f, 0.f, 0.5f };
	pRevInfo->m_fRevAngleX = (_float)D3DXToRadian(90.f);
}

void CLeftHandMove::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{

}

void CLeftHandMove::TestForExit_State(CGameObject* Owner)
{


}

void CRightHandMove::Enter_State(CGameObject* Owner)
{
	//MSG_BOX("Right Hand Enter Move");
	CPlayerHand* pHand = dynamic_cast<CPlayerHand*>(Owner);
	REVINFO* pRevInfo = pHand->Get_RevInfo();
	pRevInfo->m_vecRevTrans = { 0.f, 0.f, 0.5f };
	pRevInfo->m_fRevAngleX = (_float)D3DXToRadian(90.f);
}

void CRightHandMove::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{

}

void CRightHandMove::TestForExit_State(CGameObject* Owner)
{
}

//---------------- Player_Wash ----------------//
void CLeftHandWash::Enter_State(CGameObject* Owner)
{
}

void CLeftHandWash::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{
}

void CLeftHandWash::TestForExit_State(CGameObject* Owner)
{
}

void CRightHandWash::Enter_State(CGameObject* Owner)
{
}

void CRightHandWash::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{
}

void CRightHandWash::TestForExit_State(CGameObject* Owner)
{
}

//---------------- Player_Chop ----------------//
void CLeftHandChop::Enter_State(CGameObject* Owner)
{
	//MSG_BOX("Left Hand Enter Chop");
	dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo()->m_fRevAngleX = 270.f;

}

void CLeftHandChop::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{
	CPlayerHand* pHand = dynamic_cast<CPlayerHand*>(Owner);
	REVINFO* pRevInfo = pHand->Get_RevInfo();
	pRevInfo->m_vecRevTrans = { 0.f, 0.1f, 1.f };
}

void CLeftHandChop::TestForExit_State(CGameObject* Owner)
{
}

void CRightHandChop::Enter_State(CGameObject* Owner)
{
	//MSG_BOX("Right Hand Enter Chop");
	m_fChopTime = 0.f;
	m_fAngle = 0.f;
	m_bCw = true;
	dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo()->m_fRevAngleX = 0.f;

}

void CRightHandChop::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{
	//CDInputMgr* pInput = Engine::CDInputMgr::GetInstance();
	_float fMaxAngle = D3DXToRadian(0.f);       // 최대 각도 (0도)
	_float fMinAngle = D3DXToRadian(-90.f);     // 최소 각도 (-90도)

	CPlayerHand* pHand = dynamic_cast<CPlayerHand*>(Owner);
	REVINFO* pRevInfo = pHand->Get_RevInfo();
	pRevInfo->m_vecRevTrans = { 0.f, 0.f, 1.1f };


	if (m_bCw)
	{
		pRevInfo->m_fRevAngleX -= fTimeDelta * m_fSpeed;

		if (pRevInfo->m_fRevAngleX <= fMinAngle)
		{
			pRevInfo->m_fRevAngleX = fMinAngle;
			m_bCw = false; // 반시계 방향으로 전환
		}
	}
	else
	{
		pRevInfo->m_fRevAngleX += fTimeDelta * m_fSpeed;

		if (pRevInfo->m_fRevAngleX >= fMaxAngle)
		{
			pRevInfo->m_fRevAngleX = fMaxAngle;
			m_bCw = true; // 시계 방향으로 전환
		}
	}
	
}

void CRightHandChop::TestForExit_State(CGameObject* Owner)
{
}
