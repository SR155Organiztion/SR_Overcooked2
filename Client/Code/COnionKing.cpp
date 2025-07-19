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
	m_pTransformCom->Set_Pos(3.f, 2.f, 3.f);
	m_pTransformCom->Set_Scale({ 4.f,4.f,0.f });
	return S_OK;
}

_int COnionKing::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	if (m_bAnimation) {
		m_fFrame += 12.f * fTimeDelta;
		if (12 < m_fFrame) {
			m_fFrame = 0.f;
		}
	}

	Key_Input();

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
		if (m_bTestKey) return;
		m_bTestKey = true;
		//--------------- Body ---------------//
		if (!m_bAnimation) {
			m_bAnimation = true;
			m_fFrame = 0.f;
		}
		else {
			m_bAnimation = false;
		}

	}
	else m_bTestKey = false;

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