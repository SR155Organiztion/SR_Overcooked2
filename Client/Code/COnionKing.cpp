#include "pch.h"
#include "COnionKing.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"



COnionKing::COnionKing(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNPC(pGraphicDev)
{
}

COnionKing::~COnionKing()
{
}

HRESULT COnionKing::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	m_pTransformCom->Set_Scale({ 4.f,4.f,0.f });
	m_pTransformCom->Set_Pos(3.f, m_pTransformCom->Get_Scale().y * 0.5f, 3.f);

	m_fFrame = 0.f;

	return S_OK;
}

_int COnionKing::Update_GameObject(const _float& fTimeDelta)
{
	const _float& dt = 0.01667f;
	m_fTimeStack += dt;

	Engine::CGameObject::Update_GameObject(dt);

	Apply_State();

	if (2.f < m_fTimeStack && !m_bWalk && m_fMoveDistance == 0.f) {
		m_bWalk = true;
	}

	if (m_eCurState == ONION_DANCE) {
		m_fFrame += 12.f * dt;
		if (12 < m_fFrame) {
			m_fFrame = 0.f;
		}
	}
	Walk_Onion(dt);
	if (m_eCurState == ONION_STAND) {
		m_fFrame = 12.f;
	}

	if (m_eCurState == ONION_STAND && m_fTimeStack > 10.f && !m_bScenarioEnd) {
		m_eCurState = ONION_DANCE;
		m_bScenarioEnd = true;
	}

	//Set_Billboard();
	Key_Input();
	
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	
	return S_OK;

}

void COnionKing::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void COnionKing::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture((_int)m_fFrame);

	m_pBufferCom->Render_Buffer();
}

HRESULT COnionKing::Add_Component()
{
	CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_OnionKing"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void COnionKing::Key_Input() 
{
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_1) & 0x80)
	{
		m_eCurState = ONION_DANCE;
	}
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_2) & 0x80)
	{
		m_eCurState = ONION_STAND;
	}
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_3) & 0x80)
	{
		m_eCurState = ONION_HANDSUP;
	}
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_4) & 0x80)
	{
		m_eCurState = ONION_HAPPY;
	}
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_5) & 0x80)
	{
		m_eCurState = ONION_SAD;
	}
}

void COnionKing::Walk_Onion(const _float& dt)
{
	if (!m_bWalk)
		return;

	_matrix matWorld; m_pTransformCom->Get_World(&matWorld);
	_vec3 vRight = { matWorld._11,matWorld._12,matWorld._13 };
	D3DXVec3Normalize(&vRight, &vRight);
	_vec3 vUp = { matWorld._21,matWorld._22,matWorld._23 };
	D3DXVec3Normalize(&vUp, &vUp);

	_vec3 vLook; D3DXVec3Cross(&vLook, &vUp, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);

	
	m_pTransformCom->Move_Pos(&vLook, m_fSpeed, dt);
	m_fMoveDistance += m_fSpeed * dt;

	if (2.f < m_fMoveDistance) {
		m_bWalk = false;
		m_eCurState = ONION_STAND;
	}

}

void COnionKing::Apply_State()
{
	if (m_eCurState == m_ePreState)
		return;

	switch (m_eCurState) {
	case ONION_DANCE:
		m_fFrame = 0.f;
		break;
	case ONION_STAND:
		m_fFrame = 12.f;
		break;
	case ONION_HANDSUP:
		m_fFrame = 13.f;
		break;
	case ONION_HAPPY:
		m_fFrame = 14.f;
		break;
	case ONION_SAD:
		m_fFrame = 15.f;
		break;
	}
	m_ePreState = m_eCurState;

}

void COnionKing::Set_Billboard()
{
	//ºôº¸µå Àû¿ë
	_matrix matWorld, matView, matBill;

	m_pTransformCom->Get_World(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);
	D3DXMatrixInverse(&matView, 0, &matView);

	_vec3	vViewScale, vViewTrans;
	D3DXQUATERNION qViewRot;
	D3DXMatrixDecompose(&vViewScale, &qViewRot, &vViewTrans, &matView);
	_matrix matViewRot;  D3DXMatrixRotationQuaternion(&matViewRot, &qViewRot);

	matWorld = matViewRot * matWorld;

	m_pTransformCom->Set_World(&matWorld);
}


CGameObject* COnionKing::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGameObject* pOnionKing = new COnionKing(pGraphicDev);

	if (FAILED(pOnionKing->Ready_GameObject())) {
		Safe_Release(pOnionKing);
		MSG_BOX("NPC OninonKing Create Failed");
		return nullptr;
	}

	return pOnionKing;
}

void COnionKing::Free()
{
	Engine::CGameObject::Free();
}