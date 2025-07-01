#include "pch.h"
#include "CRealPlayer.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CTransform.h"
#include "CTimerMgr.h"

CRealPlayer::CRealPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev) 
	, m_ePlayerNum(PLAYERNUM_END), m_bGrab(false)//, m_eCurState(nullptr), m_eIdleState(nullptr), m_eMoveState(nullptr), m_eActState(nullptr)
{
}

CRealPlayer::CRealPlayer(const CGameObject& rhs)
	: Engine::CGameObject(rhs)
{
}

CRealPlayer::~CRealPlayer()
{
}

HRESULT CRealPlayer::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CCubeTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_CubeTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_PlayerTexture"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

HRESULT CRealPlayer::Ready_State()
{





	return S_OK;
}

HRESULT CRealPlayer::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	
	Engine::CTimerMgr::GetInstance()->Ready_Timer(L"Timer_Dash");

	Change_State(&m_eIdleState);

	m_pTransformCom->m_vScale = { 1.f, 2.f, 1.f };
	m_pTransformCom->Set_Pos(8.f, 2.f, 5.f);


	return S_OK;
}

_int CRealPlayer::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_eCurState->Update_State(this, fTimeDelta);
	m_eCurState->TestForExit_State(this);


	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	return S_OK;
}

void CRealPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CRealPlayer::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CRealPlayer* CRealPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRealPlayer* pPlayer = new CRealPlayer(pGraphicDev);

	if (FAILED(pPlayer->Ready_GameObject())) {
		Safe_Release(pPlayer);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}
	return pPlayer;
}

void CRealPlayer::Free()
{
}

void CRealPlayer::Change_State(CState* eState)
{
	m_eCurState = eState;
	m_eCurState->Enter_State(this);
}

void CRealPlayer::CPlayerIdle::Enter_State(Engine::CGameObject* Obj)
{
}

void CRealPlayer::CPlayerIdle::Update_State(Engine::CGameObject* Obj, const _float& fTimeDelta)
{
}

void CRealPlayer::CPlayerIdle::TestForExit_State(Engine::CGameObject* Obj)
{
	auto pPlayer = dynamic_cast<CRealPlayer*>(Obj);

	CDInputMgr* pInput = Engine::CDInputMgr::GetInstance();
	if (pInput->Get_DIKeyState(DIK_LEFT) || pInput->Get_DIKeyState(DIK_RIGHT) ||
		pInput->Get_DIKeyState(DIK_UP) || pInput->Get_DIKeyState(DIK_DOWN) ||
		pInput->Get_DIKeyState(DIK_Z)) {
		pPlayer->Change_State(&(pPlayer->m_eMoveState));
	}
}

void CRealPlayer::CPlayerMove::Enter_State(Engine::CGameObject* Obj)
{
	m_eDir = ROT_END;
	m_fDashTime = 0;

}

void CRealPlayer::CPlayerMove::Update_State(Engine::CGameObject* Obj, const _float& fTimeDelta)
{
	Check_Dir(fTimeDelta);
	Engine::CTransform* pTransformCom = dynamic_cast<Engine::CTransform*>(Obj->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	if (nullptr == pTransformCom) {
		MSG_BOX("Get TransformCom Failed");
		return;
	}


	if (!m_bDash) Rotate_Player(pTransformCom, fTimeDelta);

	if (m_bDash) { // 대쉬중일때 회전먹이는거 나중에 추가할 것
		_vec3 vLook;
		pTransformCom->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		pTransformCom->Move_Pos(&vLook, m_fSpeed * 2, fTimeDelta);
	
		m_fDashTime += fTimeDelta;

		if (0.5f <= m_fDashTime){
			m_bDash = false;
			m_fDashTime = fTimeDelta;
		}
	}
	Move_Player(pTransformCom, fTimeDelta);
}

void CRealPlayer::CPlayerMove::TestForExit_State(Engine::CGameObject* Obj)
{	
	if (true == m_bDash) return;


	auto pPlayer = dynamic_cast<CRealPlayer*>(Obj);
	CDInputMgr* pInput = Engine::CDInputMgr::GetInstance();

	if (!pInput->Get_DIKeyState(DIK_LEFT) && !pInput->Get_DIKeyState(DIK_RIGHT) &&
		!pInput->Get_DIKeyState(DIK_UP) && !pInput->Get_DIKeyState(DIK_DOWN)) {
		
		pPlayer->Change_State(&(pPlayer->m_eIdleState));
		//MSG_BOX("Enter IDLE");
	}


}

void CRealPlayer::CPlayerMove::Check_Dir(const _float& fTimeDelta)
{
	CDInputMgr* pInput = Engine::CDInputMgr::GetInstance();

	if (!m_bDash && (pInput->Get_DIKeyState(DIK_Z) & 0x80)) {
		m_bDash = true;
		m_fDashTime = fTimeDelta;
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
}

_bool CRealPlayer::CPlayerMove::Rotate_Player(Engine::CTransform* pTransformCom, const _float& fTimeDelta)
{
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

void CRealPlayer::CPlayerMove::Move_Player(Engine::CTransform* pTransformCom, const _float& fTimeDelta)
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
	}
}

void CRealPlayer::CPlayerAct::Enter_State(Engine::CGameObject* Obj)
{
}

void CRealPlayer::CPlayerAct::Update_State(Engine::CGameObject* Obj, const _float& fTimeDelta)
{
}

void CRealPlayer::CPlayerAct::TestForExit_State(Engine::CGameObject* Obj)
{
}
