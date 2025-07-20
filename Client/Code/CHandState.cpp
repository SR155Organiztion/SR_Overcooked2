#include "pch.h"
#include "CHandState.h"
#include "CPlayerHand.h"
#include "CTransform.h"
#include "CDInputMgr.h"
#include "CFSMComponent.h"
#include "CRealPlayer.h"


//---------------- Player_Idle ----------------//
void CLeftHandIdle::Enter_State(CGameObject* Owner)
{
	//MSG_BOX("Left Hand Enter Idle");
	dynamic_cast<CPlayerHand*>(Owner)->Set_UseVirtaulPivot(false);
	dynamic_cast<CPlayerHand*>(Owner)->Set_Surprised(false);
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
	dynamic_cast<CPlayerHand*>(Owner)->Set_UseVirtaulPivot(false);
	dynamic_cast<CPlayerHand*>(Owner)->Set_Surprised(false);

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

//---------------- Player_Wash ----------------//
void CLeftHandWash::Enter_State(CGameObject* Owner)
{
	//MSG_BOX("Enter Left Wash");

	CPlayerHand* pHand = dynamic_cast<CPlayerHand*>(Owner);
	REVINFO* pRevInfo = pHand->Get_RevInfo();
	pRevInfo->m_vecRevTrans = { 0.f, 0.1f, 1.f };
	pRevInfo->m_fRevAngleX = D3DXToRadian(0.f);
}

void CLeftHandWash::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{

}

void CLeftHandWash::TestForExit_State(CGameObject* Owner)
{
}

void CRightHandWash::Enter_State(CGameObject* Owner)
{
	CPlayerHand* pHand = dynamic_cast<CPlayerHand*>(Owner);
	//Redefine_matrix(pHand);
	pHand->Set_UseVirtaulPivot(true);
	
	REVINFO* pRevInfo = pHand->Get_RevInfo();
	pRevInfo->m_fRevAngleY = D3DXToRadian(0.f);
	pRevInfo->m_vecRevTrans = { 0.f, 0.f, 0.f };

	//pRevInfo->m_vecRevTrans = { 0.f, 1.f, 1.f };
	//pRevInfo->m_fRevAngleY = D3DXToRadian(0.f);
	//pRevInfo->m_fRevAngleZ = D3DXToRadian(-30.f);
}

void CRightHandWash::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{
	REVINFO* pRevInfo = dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo();
	
	pRevInfo->m_fRevAngleY -= fTimeDelta * 5.f;
}

void CRightHandWash::TestForExit_State(CGameObject* Owner)
{
}

void CRightHandWash::Redefine_matrix(CGameObject* Owner)
{
	CPlayerHand* pHand = dynamic_cast<CPlayerHand*>(Owner);

	_matrix matReLocal, matRot;

	D3DXMatrixRotationZ(&matRot, D3DXToRadian(30.f));
	matReLocal = matRot * pHand->Get_LocalMatrix();

}

//---------------- Player_Chop ----------------//
void CLeftHandChop::Enter_State(CGameObject* Owner)
{
	//MSG_BOX("Left Hand Enter Chop");
	CPlayerHand* pHand = dynamic_cast<CPlayerHand*>(Owner);
	REVINFO* pRevInfo = pHand->Get_RevInfo();
	pRevInfo->m_vecRevTrans = { 0.f, 0.1f, 1.f };
	pRevInfo->m_fRevAngleX = D3DXToRadian(0.f);

}

void CLeftHandChop::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{

}

void CLeftHandChop::TestForExit_State(CGameObject* Owner)
{
}

void CRightHandChop::Enter_State(CGameObject* Owner)
{
	//MSG_BOX("Right Hand Enter Chop");
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
			dynamic_cast<CRealPlayer*>(pHand->Get_Player())->Play_StationEffect(CURSOR_STATION, L"HitEffect");
			++m_itest;
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
	if (7 < m_itest) {
		int a = 9;
	}
}

void CRightHandChop::TestForExit_State(CGameObject* Owner)
{
}

//---------------- Player_Grab ----------------//
void CLeftHandGrab::Enter_State(CGameObject* Owner)
{
	//MSG_BOX("Enter Grab Left");
	dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo()->m_fRevAngleX = 270.f;
	dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo()->m_vecRevTrans = { 0.3f ,-0.5f , 0.8f };


}

void CLeftHandGrab::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{
}

void CLeftHandGrab::TestForExit_State(CGameObject* Owner)
{
}

void CRightHandGrab::Enter_State(CGameObject* Owner)
{
	//MSG_BOX("Enter Grab Right");
	dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo()->m_fRevAngleX = 270.f;
	dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo()->m_vecRevTrans = { -0.3f ,-0.5f , 0.8f };

}

void CRightHandGrab::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{
}

void CRightHandGrab::TestForExit_State(CGameObject* Owner)
{
}

//---------------- Player_Throw ----------------//
void CLeftHandThrow::Enter_State(CGameObject* Owner)
{
	dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo()->m_fRevAngleX = 270.f;
	dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo()->m_vecRevTrans = { 0.3f ,-0.5f , 0.8f };
	m_fThrowTime = 0;
}

void CLeftHandThrow::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{
	_float fLeftMove(0);
	m_fThrowTime += fTimeDelta;

	fLeftMove = 0.8f + 0.2f * sinf(D3DXToRadian(m_fThrowTime * 180.f / 0.5f));

	dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo()->m_vecRevTrans = { 0.3f ,-0.5f , fLeftMove };

}

void CLeftHandThrow::TestForExit_State(CGameObject* Owner)
{
	if (m_fThrowTime >= 0.5f) {
		dynamic_cast<CPlayerHand*>(Owner)->Change_OwnState("LeftHand_Idle");
	}
}

void CRightHandThrow::Enter_State(CGameObject* Owner)
{
	dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo()->m_fRevAngleX = 270.f;
	dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo()->m_vecRevTrans = { -0.3f ,-0.5f , 0.8f };
	m_fThrowTime = 0;
}

void CRightHandThrow::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{
	_float fRightMove(0);
	m_fThrowTime += fTimeDelta;
	
	fRightMove = 0.8f + 0.2f * sinf(D3DXToRadian(m_fThrowTime * 180.f / 0.5f));
	
	dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo()->m_vecRevTrans = { -0.3f ,-0.5f , fRightMove };
}

void CRightHandThrow::TestForExit_State(CGameObject* Owner)
{
	if (m_fThrowTime >= 0.5f) {
		dynamic_cast<CPlayerHand*>(Owner)->Change_OwnState("RightHand_Idle");
	}
}

void CLeftHandSurprised::Enter_State(CGameObject* Owner)
{
	CPlayerHand* pHand = dynamic_cast<CPlayerHand*>(Owner);
	pHand->Set_Surprised(true);
	REVINFO* pRevInfo = dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo();
	pRevInfo->m_fRevAngleZ = D3DXToRadian(0.f);
	pRevInfo->m_vecRevTrans = { -0.5f, 0.f, 0.6f };
}

void CLeftHandSurprised::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{
	REVINFO* pRevInfo = dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo();

	_float fMaxAngle = D3DXToRadian(30.f);       // 최대 각도 
	_float fMinAngle = D3DXToRadian(-30.f);     // 최소 각도 

	if (!m_bCw)//오른손과 반대로
	{
		pRevInfo->m_fRevAngleZ -= fTimeDelta * m_fSpeed;

		if (pRevInfo->m_fRevAngleZ <= fMinAngle)
		{
			pRevInfo->m_fRevAngleZ = fMinAngle;
			m_bCw = true; // 오른손과 반대로
		}
	}
	else
	{
		pRevInfo->m_fRevAngleZ += fTimeDelta * m_fSpeed;

		if (pRevInfo->m_fRevAngleZ >= fMaxAngle)
		{
			pRevInfo->m_fRevAngleZ = fMaxAngle;
			m_bCw = false; // 오른손과 반대로
		}
	}
}

void CLeftHandSurprised::TestForExit_State(CGameObject* Owner)
{
}

void CRightHandSurprised::Enter_State(CGameObject* Owner)
{
	CPlayerHand* pHand = dynamic_cast<CPlayerHand*>(Owner);
	pHand->Set_Surprised(true);
	REVINFO* pRevInfo = dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo();
	pRevInfo->m_fRevAngleZ = D3DXToRadian(0.f);
	pRevInfo->m_vecRevTrans = { 0.5f, 0.f, 0.6f };
}

void CRightHandSurprised::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{
	REVINFO* pRevInfo = dynamic_cast<CPlayerHand*>(Owner)->Get_RevInfo();

	_float fMaxAngle = D3DXToRadian(30.f);       // 최대 각도 
	_float fMinAngle = D3DXToRadian(-30.f);     // 최소 각도 

	if (m_bCw)
	{
		pRevInfo->m_fRevAngleZ -= fTimeDelta * m_fSpeed;
	
		if (pRevInfo->m_fRevAngleZ <= fMinAngle)
		{
			pRevInfo->m_fRevAngleZ = fMinAngle;
			m_bCw = false; // 반시계 방향으로 전환
		}
	}
	else
	{
		pRevInfo->m_fRevAngleZ += fTimeDelta * m_fSpeed;
	
		if (pRevInfo->m_fRevAngleZ >= fMaxAngle)
		{
			pRevInfo->m_fRevAngleZ = fMaxAngle;
			m_bCw = true; // 시계 방향으로 전환
		}
	}
}

void CRightHandSurprised::TestForExit_State(CGameObject* Owner)
{
}
