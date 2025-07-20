#include "pch.h"
#include "CBus.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CEffectMgr.h"

CBus::CBus(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_eDir(PLAYERROT_END)
{
}

CBus::~CBus()
{
}

HRESULT CBus::Add_Component()
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


HRESULT CBus::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CBus::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	Check_CoolTime(fTimeDelta);
	Key_Input();
	Rotate_Bus(fTimeDelta);
	Dash_Bus(fTimeDelta);
	Move_Bus(fTimeDelta);



	return S_OK;

}

void CBus::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CBus::Render_GameObject() 
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(2);

	Engine::CGameObject::Set_Material();

	m_pBufferCom->Render_Buffer();
}

void CBus::Set_FirstPos(_float x, _float y, _float z)
{
	if (!m_pTransformCom) {
		MSG_BOX("Bus First Pos Set Failed");
		return;
	}
	m_pTransformCom->Set_Pos(x, y, z);
}

void CBus::Set_FirstScale(_float x, _float y, _float z)
{
	if (!m_pTransformCom) {
		MSG_BOX("Bus First Scale Set Failed");
		return;
	}

	_vec3 vScale = { x,y,z };

	m_pTransformCom->Set_Scale(vScale);
}


void CBus::Key_Input()
{
	CDInputMgr* pInput = CDInputMgr::GetInstance();
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

void CBus::Check_CoolTime(const _float& dt)
{
	m_eDir = PLAYERROT_END;

	CDInputMgr* pInput = Engine::CDInputMgr::GetInstance();

	if (m_bDashCool) { // 대쉬 쿨타임 체크
		m_fDashCoolTime += dt;
		if (0.3f < m_fDashCoolTime) {
			m_fDashCoolTime = 0;
			m_bDashCool = false;
		}
	}
	if (m_bEffect) {
		m_fEffect += dt;
		if (0.15f < m_fEffect) {
			m_bEffect = false;
			m_fEffect = 0;
		}
	}
}

void CBus::Move_Bus(const _float& dt)
{
	if (m_bDash)
		return;

	_vec3 vPos{};
	switch (m_eDir) {
	case PLAYER_L:
		vPos = { -1.f, 0.f, 0.f };
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vPos, &vPos), m_fSpeed, dt);
		break;
	case PLAYER_R:
		vPos = { 1.f, 0.f, 0.f };
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vPos, &vPos), m_fSpeed, dt);
		break;
	case PLAYER_U:
		vPos = { 0.f, 0.f, 1.f };
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vPos, &vPos), m_fSpeed, dt);
		break;
	case PLAYER_D:
		vPos = { 0.f, 0.f, -1.f };
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vPos, &vPos), m_fSpeed, dt);
		break;
	case PLAYER_LD:
		vPos = { -1.f, 0.f, -1.f };
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vPos, &vPos), m_fSpeed, dt);
		break;
	case PLAYER_RD:
		vPos = { 1.f, 0.f, -1.f };
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vPos, &vPos), m_fSpeed, dt);
		break;
	case PLAYER_LU:
		vPos = { -1.f, 0.f, 1.f };
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vPos, &vPos), m_fSpeed, dt);
		break;
	case PLAYER_RU:
		vPos = { 1.f, 0.f, 1.f };
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vPos, &vPos), m_fSpeed, dt);
		break;
	case PLAYERROT_END:
		return;

	}
	if (!m_bEffect) {
		CEffectMgr::GetInstance()->Play_Effect(L"TestEffect", this);
		m_bEffect = true;
		m_fEffect = 0;
	}
}

void CBus::Rotate_Bus(const _float& dt)
{
	if (m_eDir == PLAYERROT_END) 
		return;

	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
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

	m_pTransformCom->Rotation(ROT_Y, fAngle * dt * 8);// fTimeDelta에 값 곱할수록 빠르게 돌아감 

}

void CBus::Dash_Bus(const _float& dt)
{
	if (!m_bDash)
		return;

	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	m_pTransformCom->Move_Pos(&vLook, m_fSpeed * 2.f, dt);

	m_fDashTime += dt;

	if (0.2f <= m_fDashTime) {
		m_bDash = false;
		m_fDashTime = 0;
		m_fDashCoolTime = 0.f;
		m_bDashCool = true;
	}
}

CBus* CBus::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBus* pBus = new CBus(pGraphicDev);

	if (FAILED(pBus->Ready_GameObject())) {
		Safe_Release(pBus);
		MSG_BOX("Bus Create Failed");
		return nullptr;
	}
	return pBus;
}

void CBus::Free()
{
	Engine::CGameObject::Free();
}
