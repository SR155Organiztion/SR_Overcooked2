#include "pch.h"
#include "CUi_GameLoding.h"

CUi_GameLoding::CUi_GameLoding():CUi_Gauge(nullptr)
{
}

CUi_GameLoding::CUi_GameLoding(LPDIRECT3DDEVICE9 pGraphicDev):CUi_Gauge(pGraphicDev)
{
}

CUi_GameLoding::CUi_GameLoding(const CGameObject& rhs):CUi_Gauge(rhs)
{
}

CUi_GameLoding::~CUi_GameLoding()
{
}

HRESULT CUi_GameLoding::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

		m_tData.m_vPos = D3DXVECTOR3(0, 0, 0);
		m_tData.m_fXScale = 0.8f;
		m_tData.m_fYScale = 0.6;
		return S_OK;
}

int CUi_GameLoding::Update_GameObject(const _float& _fTimeDelta)
{
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	return iExit;
}

void CUi_GameLoding::LateUpdate_GameObject()
{
}

void CUi_GameLoding::Render_GameObject()
{
	if (m_tData.m_bVisible)
	{
		/*float percent = (float)m_tData.m_dwTime / (float)m_tData.m_dwLimitTime;*/
		//float percent = m_tData.m_dwTime;
		//percent = min(percent, 1.0f); 
		float m_pGauge = (40 * 8.0f);
		SetRect(&m_tData.m_pLoadingRect, 0, 0, m_pGauge, 600);

		_vec3 m_pCenter2 = {0.f,0.f,0.f};
		//_vec3 m_vPosTest = { 100.f, 50.f, 1.f };

		m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, &m_pCenter2, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/GameLoading_Back.png");
		//m_pSpriteCom2->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, &m_pCenter2, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/GameLoading0.png");
		m_pSpriteCom3->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, &m_tData.m_pLoadingRect, &m_pCenter2, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/GameLoading1.png");
	}

}

HRESULT CUi_GameLoding::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_GameLoading_Back"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });

	pComponent = m_pSpriteCom2 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_GameLoading"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite2", pComponent });

	pComponent = m_pSpriteCom3 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_GameLoading"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite3", pComponent });

	return S_OK;
}

void CUi_GameLoding::Make_GameLoding(bool _bVisible)
{
	m_tData.m_bVisible = _bVisible;
	m_tData.m_dwLimitTime = 10000;
}



void CUi_GameLoding::Free()
{
}
