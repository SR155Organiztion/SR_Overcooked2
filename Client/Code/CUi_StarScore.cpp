#include "pch.h"
#include "CUi_StarScore.h"


CUi_StarScore::CUi_StarScore(LPDIRECT3DDEVICE9 pGraphicDev): CUi(pGraphicDev)
{
}

CUi_StarScore::CUi_StarScore(const CGameObject& rhs): CUi(rhs)
{
}

CUi_StarScore::~CUi_StarScore()
{
	Free();
}

HRESULT CUi_StarScore::Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	m_tData.m_fXScale = 0.5f;
	m_tData.m_fYScale = 0.5f;
	m_tData.m_vPos = D3DXVECTOR3(0, 0, 0);

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUi_StarScore::Update_GameObject(const _float& _fTimeDelta)
{
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	return iExit;
}

void CUi_StarScore::LateUpdate_GameObject()
{

}

void CUi_StarScore::Render_GameObject()
{
	m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Complete1.png");
	m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Complete_Score0.png");

	switch (m_iNumber)
	{
	case 1:
		m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Complete_Score1.png");
		break;

	case 2:
		m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Complete_Score2.png");
		break;

	case 3:
		m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Complete_Score3.png");
		break;
	}
}

HRESULT CUi_StarScore::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Complete"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Sprite", pComponent });

	pComponent = m_pSpriteCom2 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StarSore"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Sprite2", pComponent });

}

void CUi_StarScore::Free()
{
}

