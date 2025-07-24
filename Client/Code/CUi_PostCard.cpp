#include "pch.h"
#include "CUi_PostCard.h"

CUi_PostCard::CUi_PostCard(): CUi()
{
}

CUi_PostCard::CUi_PostCard(LPDIRECT3DDEVICE9 pGraphicDev): CUi(pGraphicDev)
{
}

CUi_PostCard::CUi_PostCard(const CUi_PostCard& rhs): CUi(rhs)
{
}

CUi_PostCard::~CUi_PostCard()
{
}

HRESULT CUi_PostCard::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	Make_PostCard(true); 

	m_tData.m_vPos = { 0,0,0 }; 
	m_tData.m_fXScale = 0.8f; 
	m_tData.m_fYScale = 0.6f; 

	return S_OK;
}

int CUi_PostCard::Update_GameObject(const _float& _fTimeDelta)
{
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return iExit;
}

void CUi_PostCard::LateUpdate_GameObject(const _float& _fTimeDelta)
{
}

void CUi_PostCard::Render_GameObject()
{
	if (!m_tData.m_bVisible)
	return;
	m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/Menu/Postcard.png");

}

HRESULT CUi_PostCard::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_PostCard"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });

	return S_OK;
}

void CUi_PostCard::Make_PostCard(bool _bVisible)
{
	m_tData.m_bVisible = _bVisible;
}

void CUi_PostCard::Free()
{
}