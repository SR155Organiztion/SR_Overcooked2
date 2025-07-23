#include "pch.h"
#include "CPlayerState.h"
#include "CRealPlayer.h"
#include "CGameObject.h"
#include "CDInputMgr.h"
#include "CFSMComponent.h"
#include "CTransform.h"

#include "CEffectMgr.h"
#include "CSoundMgr.h"
#include "CUtil.h"

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
	
	if (PLAYER_1P == pPlayer->Get_PlayerNum()) {
		if (pInput->Get_DIKeyState(DIK_F) || pInput->Get_DIKeyState(DIK_H) ||
			pInput->Get_DIKeyState(DIK_T) || pInput->Get_DIKeyState(DIK_G) ||
			pInput->Get_DIKeyState(DIK_C)) {
			pPlayer->Change_PlayerState("Player_Move");
		}
	}
	if (PLAYER_2P == pPlayer->Get_PlayerNum()) {
		if (pInput->Get_DIKeyState(DIK_LEFT) || pInput->Get_DIKeyState(DIK_RIGHT) ||
			pInput->Get_DIKeyState(DIK_UP) || pInput->Get_DIKeyState(DIK_DOWN) ||
			pInput->Get_DIKeyState(DIK_SLASH)) {
			pPlayer->Change_PlayerState("Player_Move");
		}
	}
}

void CPlayerMove::Enter_State(CGameObject* Owner)
{
	m_eDir = PLAYERROT_END;
	m_fDashTime = 0;
	m_bDash = false;
	m_bDashCool = false;

	//이펙트 테스트용
	m_fCloudEffect = 0.f;
	m_bCloudEffect = false;
	m_pOwner = Owner;
}

void CPlayerMove::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{

	Engine::CTransform* pTransformCom = dynamic_cast<Engine::CTransform*>(Owner->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	if (nullptr == pTransformCom) {
		MSG_BOX("Get TransformCom Failed");
		return;
	}

	Check_Dir(fTimeDelta, dynamic_cast<CRealPlayer*>(Owner)->Get_PlayerNum());

	if (!m_bDash) {}

	Rotate_Player(pTransformCom, fTimeDelta);

	if (m_bDash) { 
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


	if (PLAYER_1P == pPlayer->Get_PlayerNum()) {
		if (!pInput->Get_DIKeyState(DIK_F) && !pInput->Get_DIKeyState(DIK_H) &&
			!pInput->Get_DIKeyState(DIK_T) && !pInput->Get_DIKeyState(DIK_G) &&
			!pInput->Get_DIKeyState(DIK_C)) {
			pPlayer->Change_PlayerState("Player_Idle");
		}
	}
	if (PLAYER_2P == pPlayer->Get_PlayerNum()) {
		if (!pInput->Get_DIKeyState(DIK_LEFT) && !pInput->Get_DIKeyState(DIK_RIGHT) &&
			!pInput->Get_DIKeyState(DIK_UP) && !pInput->Get_DIKeyState(DIK_DOWN) &&
			!pInput->Get_DIKeyState(DIK_SLASH)) {
			pPlayer->Change_PlayerState("Player_Idle");
		}
	}


}

void CPlayerMove::Check_Dir(const _float& fTimeDelta, PLAYER_NUM ePlayer)
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
	if (m_bCloudEffect) {
		m_fCloudEffect += fTimeDelta;
		if (0.15f < m_fCloudEffect) {
			m_bCloudEffect = false;
			m_fCloudEffect = 0;
		}
	}
	if (PLAYER_2P == ePlayer) {
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
		if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_SLASH) & 0x80)
		{
			if (m_bCheckKey) return;
			m_bCheckKey = true;
			//--------------- Body ---------------//
			if (m_bDash || m_bDashCool) return;
			Dash_Effect();
			m_bDash = true;
			m_fDashTime = 0;
		}
		else m_bCheckKey = false;
	}

	if (PLAYER_1P == ePlayer) {
		// 왼쪽 키 관련
		if (pInput->Get_DIKeyState(DIK_F) & 0x80) {
			if (pInput->Get_DIKeyState(DIK_T)) {
				m_eDir = PLAYER_LU;
				//MSG_BOX("LU");
			}
			else if (pInput->Get_DIKeyState(DIK_G)) {
				m_eDir = PLAYER_LD;
				//MSG_BOX("LD");
			}
			else {
				m_eDir = PLAYER_L;
				//MSG_BOX("L");
			}
		}
		// 오른쪽 키 관련
		if (pInput->Get_DIKeyState(DIK_H) & 0x80) {
			if (pInput->Get_DIKeyState(DIK_T)) {
				m_eDir = PLAYER_RU;
				//MSG_BOX("RU");
			}
			else if (pInput->Get_DIKeyState(DIK_G)) {
				m_eDir = PLAYER_RD;
				//MSG_BOX("RD");
			}
			else {
				m_eDir = PLAYER_R;
				//MSG_BOX("R");
			}
		}
		if (pInput->Get_DIKeyState(DIK_T) & 0x80) {
			if (pInput->Get_DIKeyState(DIK_F)) {
				m_eDir = PLAYER_LU;
				//MSG_BOX("LU");
			}
			else if (pInput->Get_DIKeyState(DIK_H)) {
				m_eDir = PLAYER_RU;
				//MSG_BOX("RU");
			}
			else {
				m_eDir = PLAYER_U;
				//MSG_BOX("U");
			}
		}
		if (pInput->Get_DIKeyState(DIK_G) & 0x80) {
			if (pInput->Get_DIKeyState(DIK_F)) {
				m_eDir = PLAYER_LD;
				//MSG_BOX("LD");
			}
			else if (pInput->Get_DIKeyState(DIK_H)) {
				m_eDir = PLAYER_RD;
				//MSG_BOX("RD");
			}
			else {
				m_eDir = PLAYER_D;
				//MSG_BOX("D");
			}
		}
		if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_C) & 0x80)
		{
			if (m_bCheckKey) return;
			m_bCheckKey = true;
			//--------------- Body ---------------//
			if (m_bDash || m_bDashCool) return;

			Dash_Effect();
			m_bDash = true;
			m_fDashTime = 0;
		}
		else m_bCheckKey = false;
	}
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
	if (!m_bCloudEffect) {
		CEffectMgr::GetInstance()->Play_Effect(L"CloudEffect", m_pOwner);
		m_bCloudEffect = true;
		m_fCloudEffect = 0;
	}

}

void CPlayerMove::Dash_Effect()
{
	CTransform* pTransform = dynamic_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	
	_matrix	matPlayerWorld; pTransform->Get_World(&matPlayerWorld);
	_vec3 vPlayerPos; pTransform->Get_Info(INFO_POS, &vPlayerPos);
	_vec3 vPlayerRight = { matPlayerWorld._11, matPlayerWorld._12, matPlayerWorld._13 };
	D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);
	_vec3 vPlayerLook = { matPlayerWorld._31, matPlayerWorld._32, matPlayerWorld._33 };
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
	_vec3 vPlayerRPos = vPlayerPos + vPlayerRight * 0.5f;
	_vec3 vPlayerLPos = vPlayerPos - vPlayerRight * 0.5f;
	_vec3 vPlayerFrontPos = vPlayerPos + vPlayerLook * 0.7f;
	_vec3 vPlayerFrontPos1 = vPlayerPos + vPlayerLook * 1.2f;


	CEffectMgr::GetInstance()->Play_Effect_Pos(L"CloudEffect", vPlayerPos);
	CEffectMgr::GetInstance()->Play_Effect_Pos(L"CloudEffect", vPlayerFrontPos);
	CEffectMgr::GetInstance()->Play_Effect_Pos(L"CloudEffect", vPlayerFrontPos1);
	CEffectMgr::GetInstance()->Play_Effect_Pos(L"CloudEffect", vPlayerRPos);
	CEffectMgr::GetInstance()->Play_Effect_Pos(L"CloudEffect", vPlayerLPos);

	vector<SOUND_ID> m_vecSound;
	m_vecSound.push_back(SOUND_ID::PLAYER_DASH1);
	m_vecSound.push_back(SOUND_ID::PLAYER_DASH2);
	m_vecSound.push_back(SOUND_ID::PLAYER_DASH3);
	m_vecSound.push_back(SOUND_ID::PLAYER_DASH4);
	_int RandomDash = CUtil::Make_Random<int>(0, m_vecSound.size());
	CSoundMgr::GetInstance()->Play_Sound(m_vecSound[RandomDash], PLAYER_CHANNEL);

}

void CPlayerAct::Enter_State(CGameObject* Owner)
{
	//MSG_BOX("Enter Player Act");
	m_fCurAngle = 0.f;
	m_fPreAngle = 0.f;
}

void CPlayerAct::Update_State(CGameObject* Owner, const _float& fTimeDelta)
{
	Set_LookAtStation(Owner, fTimeDelta);
}

void CPlayerAct::TestForExit_State(CGameObject* Owner)
{
	auto pPlayer = dynamic_cast<CRealPlayer*>(Owner);
	CDInputMgr* pInput = Engine::CDInputMgr::GetInstance();

	if (PLAYER_1P == pPlayer->Get_PlayerNum()) {
		if (pInput->Get_DIKeyState(DIK_F) || pInput->Get_DIKeyState(DIK_H) ||
			pInput->Get_DIKeyState(DIK_T) || pInput->Get_DIKeyState(DIK_G) ||
			pInput->Get_DIKeyState(DIK_C)) {
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
	if (PLAYER_2P == pPlayer->Get_PlayerNum()) {
		if (pInput->Get_DIKeyState(DIK_LEFT) || pInput->Get_DIKeyState(DIK_RIGHT) ||
			pInput->Get_DIKeyState(DIK_UP) || pInput->Get_DIKeyState(DIK_DOWN) ||
			pInput->Get_DIKeyState(DIK_SLASH)) {
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
	//if (pInput->Get_DIKeyState(DIK_LEFT) || pInput->Get_DIKeyState(DIK_RIGHT) ||
	//	pInput->Get_DIKeyState(DIK_UP) || pInput->Get_DIKeyState(DIK_DOWN) ||
	//	pInput->Get_DIKeyState(DIK_Z)) {
	//	
	//	std::string CurState = dynamic_cast<CFSMComponent*>(pPlayer->Get_Hand(HAND_LEFT)->Get_Component(ID_DYNAMIC, L"Com_FSM"))->GerCurrStateName();
	//
	//	if ("LeftHand_Chop" == CurState) {
	//		pPlayer->Escape_Act(ACT_CHOP, true, "Player_Move");
	//		//MSG_BOX("Escape Chop");
	//	}
	//	else if ("LeftHand_Wash" == CurState) {
	//		pPlayer->Escape_Act(ACT_WASH, true, "Player_Move");
	//		//MSG_BOX("Escape Wash");
	//	}
	//}
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
	m_fCurAngle = fAngle;

	if (m_fPreAngle < 0) {
		if (m_fCurAngle > 0) {
			return;
		}
	}

	pPlayerTransCom->Rotation(ROT_Y, fAngle * dt * 6);// fTimeDelta에 값 곱할수록 빠르게 돌아감 
	
	m_fPreAngle = m_fCurAngle;

}
