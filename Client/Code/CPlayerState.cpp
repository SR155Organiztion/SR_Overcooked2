#include "pch.h"
#include "CPlayerState.h"
#include "CRealPlayer.h"
#include "CGameObject.h"
#include "CDInputMgr.h"
#include "CFSMComponent.h"
#include "CTransform.h"

#include "CEffectMgr.h"


void CPlayerIdle::Enter_State(CGameObject* Owner)
{
}

void CPlayerIdle::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{
}

void CPlayerIdle::TestForExit_State(CGameObject* Owner)
{
	auto pPlayer = dynamic_cast<CRealPlayer*>(Owner);

	CDInputMgr* pInput = Engine::CDInputMgr::GetInstance();
	if (pInput->Get_DIKeyState(DIK_LEFT) || pInput->Get_DIKeyState(DIK_RIGHT) ||
		pInput->Get_DIKeyState(DIK_UP) || pInput->Get_DIKeyState(DIK_DOWN) ||
		pInput->Get_DIKeyState(DIK_Z)) {
		pPlayer->Change_PlayerState("Player_Move");
	}
}

void CPlayerMove::Enter_State(CGameObject* Owner)
{
	m_eDir = PLAYERROT_END;
	m_fDashTime = 0;
	m_bDash = false;
	m_bDashCool = false;

	//이펙트 테스트용
	m_fTestEffect = 0.f;
	m_bTestEffect = false;
	m_pOwner = Owner;
}

void CPlayerMove::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{

	Engine::CTransform* pTransformCom = dynamic_cast<Engine::CTransform*>(Owner->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	if (nullptr == pTransformCom) {
		MSG_BOX("Get TransformCom Failed");
		return;
	}

	Check_Dir(fTimeDelta);

	if (!m_bDash) {}

	Rotate_Player(pTransformCom, fTimeDelta);

	if (m_bDash) { // 대쉬중일때 회전먹이는거 나중에 추가할 것
		_vec3 vLook;
		pTransformCom->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		pTransformCom->Move_Pos(&vLook, m_fSpeed * 2.f, fTimeDelta);

		m_fDashTime += fTimeDelta;

		if (0.2f <= m_fDashTime) {
			m_bDash = false;
			m_fDashTime = 0;
			m_fDashCoolTime = 0.f;
			m_bDashCool = true;
		}
	}
	if (!m_bDash)
		Move_Player(pTransformCom, fTimeDelta);
}

void CPlayerMove::TestForExit_State(CGameObject* Owner)
{
	if (true == m_bDash) return;

	auto pPlayer = dynamic_cast<CRealPlayer*>(Owner);
	CDInputMgr* pInput = Engine::CDInputMgr::GetInstance();

	if (!pInput->Get_DIKeyState(DIK_LEFT) && !pInput->Get_DIKeyState(DIK_RIGHT) &&
		!pInput->Get_DIKeyState(DIK_UP) && !pInput->Get_DIKeyState(DIK_DOWN) &&
		!pInput->Get_DIKeyState(DIK_Z)) {
		pPlayer->Change_PlayerState("Player_Idle");

		//MSG_BOX("Escape");
	}
}

void CPlayerMove::Check_Dir(const _float& fTimeDelta)
{
	m_eDir = PLAYERROT_END;

	CDInputMgr* pInput = Engine::CDInputMgr::GetInstance();

	if (m_bDashCool) { // 대쉬 쿨타임 체크
		m_fDashCoolTime += fTimeDelta;
		if (0.3f < m_fDashCoolTime) {
			m_fDashCoolTime = 0;
			m_bDashCool = false;
		}
	}
	if (m_bTestEffect) {
		m_fTestEffect += fTimeDelta;
		if (0.1f < m_fTestEffect) {
			m_bTestEffect = false;
			m_fTestEffect = 0;
		}
	}

	// 왼쪽 키 관련
	if (pInput->Get_DIKeyState(DIK_LEFT) & 0x80) {
		if (pInput->Get_DIKeyState(DIK_UP)) {
			m_eDir = PLAYER_LU;
			//MSG_BOX("LU");
		}
		else if (pInput->Get_DIKeyState(DIK_DOWN)) {
			m_eDir = PLAYER_LD;
			//MSG_BOX("LD");
		}
		else {
			m_eDir = PLAYER_L;
			//MSG_BOX("L");
		}
	}
	// 오른쪽 키 관련
	if (pInput->Get_DIKeyState(DIK_RIGHT) & 0x80) {
		if (pInput->Get_DIKeyState(DIK_UP)) {
			m_eDir = PLAYER_RU;
			//MSG_BOX("RU");
		}
		else if (pInput->Get_DIKeyState(DIK_DOWN)) {
			m_eDir = PLAYER_RD;
			//MSG_BOX("RD");
		}
		else {
			m_eDir = PLAYER_R;
			//MSG_BOX("R");
		}
	}
	if (pInput->Get_DIKeyState(DIK_UP) & 0x80) {
		if (pInput->Get_DIKeyState(DIK_LEFT)) {
			m_eDir = PLAYER_LU;
			//MSG_BOX("LU");
		}
		else if (pInput->Get_DIKeyState(DIK_RIGHT)) {
			m_eDir = PLAYER_RU;
			//MSG_BOX("RU");
		}
		else {
			m_eDir = PLAYER_U;
			//MSG_BOX("U");
		}
	}
	if (pInput->Get_DIKeyState(DIK_DOWN) & 0x80) {
		if (pInput->Get_DIKeyState(DIK_LEFT)) {
			m_eDir = PLAYER_LD;
			//MSG_BOX("LD");
		}
		else if (pInput->Get_DIKeyState(DIK_RIGHT)) {
			m_eDir = PLAYER_RD;
			//MSG_BOX("RD");
		}
		else {
			m_eDir = PLAYER_D;
			//MSG_BOX("D");
		}
	}
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_Z) & 0x80)
	{
		if (m_bCheckKey) return;
		m_bCheckKey = true;
		//--------------- Body ---------------//
		if (m_bDash || m_bDashCool) return;


		m_bDash = true;
		m_fDashTime = 0;
	}
	else m_bCheckKey = false;

}

_bool CPlayerMove::Rotate_Player(CTransform* pTransformCom, const _float& fTimeDelta)
{
	if (m_eDir == PLAYERROT_END) return false;

	_vec3 vLook;
	pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vTo{};

	switch (m_eDir) {
	case PLAYER_L:
		vTo = { -1.f, 0.f, 0.f };
		break;
	case PLAYER_R:
		vTo = { 1.f, 0.f, 0.f };
		break;
	case PLAYER_U:
		vTo = { 0.f, 0.f, 1.f };
		break;
	case PLAYER_D:
		vTo = { 0.f, 0.f, -1.f };
		break;
	case PLAYER_LD:
		vTo = { -1.f, 0.f, -1.f };
		D3DXVec3Normalize(&vTo, &vTo);
		break;
	case PLAYER_RD:
		vTo = { 1.f, 0.f, -1.f };
		D3DXVec3Normalize(&vTo, &vTo);
		break;
	case PLAYER_LU:
		vTo = { -1.f, 0.f, 1.f };
		D3DXVec3Normalize(&vTo, &vTo);
		break;
	case PLAYER_RU:
		vTo = { 1.f, 0.f, 1.f };
		D3DXVec3Normalize(&vTo, &vTo);
		break;

	}
	_float fDot = D3DXVec3Dot(&vLook, &vTo);
	fDot = max(-1, min(1.f, fDot));
	_float fAngle = acosf(fDot);

	D3DXVECTOR3 vCross;
	D3DXVec3Cross(&vCross, &vLook, &vTo);
	if (vCross.y < 0)
		fAngle = -fAngle;

	pTransformCom->Rotation(ROT_Y, fAngle * fTimeDelta * 8);// fTimeDelta에 값 곱할수록 빠르게 돌아감 

	return false;

}

void CPlayerMove::Move_Player(CTransform* pTransformCom, const _float& fTimeDelta)
{
	_vec3 vPos{};

	switch (m_eDir) {
	case PLAYER_L:
		vPos = { -1.f, 0.f, 0.f };
		pTransformCom->Move_Pos(D3DXVec3Normalize(&vPos, &vPos), m_fSpeed, fTimeDelta);
		break;
	case PLAYER_R:
		vPos = { 1.f, 0.f, 0.f };
		pTransformCom->Move_Pos(D3DXVec3Normalize(&vPos, &vPos), m_fSpeed, fTimeDelta);
		break;
	case PLAYER_U:
		vPos = { 0.f, 0.f, 1.f };
		pTransformCom->Move_Pos(D3DXVec3Normalize(&vPos, &vPos), m_fSpeed, fTimeDelta);
		break;
	case PLAYER_D:
		vPos = { 0.f, 0.f, -1.f };
		pTransformCom->Move_Pos(D3DXVec3Normalize(&vPos, &vPos), m_fSpeed, fTimeDelta);
		break;
	case PLAYER_LD:
		vPos = { -1.f, 0.f, -1.f };
		pTransformCom->Move_Pos(D3DXVec3Normalize(&vPos, &vPos), m_fSpeed, fTimeDelta);
		break;
	case PLAYER_RD:
		vPos = { 1.f, 0.f, -1.f };
		pTransformCom->Move_Pos(D3DXVec3Normalize(&vPos, &vPos), m_fSpeed, fTimeDelta);
		break;
	case PLAYER_LU:
		vPos = { -1.f, 0.f, 1.f };
		pTransformCom->Move_Pos(D3DXVec3Normalize(&vPos, &vPos), m_fSpeed, fTimeDelta);
		break;
	case PLAYER_RU:
		vPos = { 1.f, 0.f, 1.f };
		pTransformCom->Move_Pos(D3DXVec3Normalize(&vPos, &vPos), m_fSpeed, fTimeDelta);
		break;
	case PLAYERROT_END:
		return;

	}
	if (!m_bTestEffect) {
		CEffectMgr::GetInstance()->Play_Effect(L"TestEffect", m_pOwner);
		m_bTestEffect = true;
		m_fTestEffect = 0;
	}

}

void CPlayerAct::Enter_State(CGameObject* Owner)
{
	//MSG_BOX("Enter Player Act");
	m_bRot = false;
}

void CPlayerAct::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{
	Set_LookAtStation(Owner, fTimeDelta);
}

void CPlayerAct::TestForExit_State(CGameObject* Owner)
{
	auto pPlayer = dynamic_cast<CRealPlayer*>(Owner);
	CDInputMgr* pInput = Engine::CDInputMgr::GetInstance();
	if (pInput->Get_DIKeyState(DIK_LEFT) || pInput->Get_DIKeyState(DIK_RIGHT) ||
		pInput->Get_DIKeyState(DIK_UP) || pInput->Get_DIKeyState(DIK_DOWN) ||
		pInput->Get_DIKeyState(DIK_Z)) {
		
		std::string CurState = dynamic_cast<CFSMComponent*>(pPlayer->Get_Hand(HAND_LEFT)->Get_Component(ID_DYNAMIC, L"Com_FSM"))->GerCurrStateName();

		if ("LeftHand_Chop" == CurState) {
			pPlayer->Escape_Act(ACT_CHOP, true, "Player_Move");
			//MSG_BOX("Escape Chop");
		}
		else if ("LeftHand_Wash" == CurState) {
			pPlayer->Escape_Act(ACT_WASH, true, "Player_Move");
			//MSG_BOX("Escape Wash");
		}
	}
}

void CPlayerAct::Set_LookAtStation(CGameObject* Owner, const _float& dt)
{
	CGameObject* pStation = dynamic_cast<CRealPlayer*>(Owner)->Get_CursorStation();
	if (!pStation)
		return;

	_vec3 vStationPos; 

	dynamic_cast<CTransform*>(pStation->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vStationPos);
	
	CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Owner->Get_Component(ID_DYNAMIC, L"Com_Transform"));

	_vec3 vPlayerLook; pPlayerTransCom->Get_Info(INFO_LOOK, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
	_vec3 vPlayerPos; pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
	_vec3 vLookStation = vStationPos - vPlayerPos;
	D3DXVec3Normalize(&vLookStation, &vLookStation);
	vPlayerLook.y = 0.f;
	vLookStation.y = 0.f;

	_float fDot = D3DXVec3Dot(&vPlayerLook, &vLookStation);
	fDot = max(-1, min(1.f, fDot));
	_float fAngle = acosf(fDot);

	_vec3 vCross;
	D3DXVec3Cross(&vCross, &vPlayerLook, &vLookStation);
	if (vCross.y < 0)
		fAngle = -fAngle;

	if (fabsf(fAngle) < 0.1f) {
		return;
	}
	pPlayerTransCom->Rotation(ROT_Y, fAngle * dt * 4);// fTimeDelta에 값 곱할수록 빠르게 돌아감 
}
