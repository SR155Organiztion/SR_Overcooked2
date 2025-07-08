#include "pch.h"
#include "CUi_Sashimi.h"

CUi_Sashimi::CUi_Sashimi(LPDIRECT3DDEVICE9 pGraphicDev):CUi_Order(pGraphicDev)
{
}

CUi_Sashimi::CUi_Sashimi(const CUi_Sashimi& rhs): CUi_Order(rhs)
{
}

CUi_Sashimi::~CUi_Sashimi()
{
}


HRESULT CUi_Sashimi::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, OBJECT_TYPE _type)
{
	m_dwStartTime = GetTickCount64();
	m_dwLimitTime = 10000;
	m_eObjectType = _type;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_dwTime = GetTickCount64();

	float remaining = (m_dwLimitTime > (m_dwTime - m_dwStartTime)) ? (m_dwLimitTime - (m_dwTime - m_dwStartTime)) : 0;
	m_iminute = (int)(remaining / 1000) / 60;
	m_iseconds = (int)(remaining / 1000) % 60;

	m_vPos = D3DXVECTOR3(10, 10, 0);
	m_tXScale = 1.0f;
	m_tYScale = 1.0f;

	return S_OK;
}


int CUi_Sashimi::Update_GameObject(const _float& _fTimeDelta)
{
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	if (m_dwTime - m_dwStartTime >= m_dwLimitTime)
	{
		//타임 오버
	}
	return iExit;
}


void CUi_Sashimi::LateUpdate_GameObject(const _float& _fTimeDelta)
{

}

void CUi_Sashimi::Render_GameObject()
{

	m_dwTime = GetTickCount64();
	float remaining = (m_dwLimitTime > (m_dwTime - m_dwStartTime)) ? (m_dwLimitTime - (m_dwTime - m_dwStartTime)) : 0;
	m_iminute = (int)(remaining / 1000) / 60;
	m_iseconds = (int)(remaining / 1000) % 60;
	
	/*switch*/ 
	
		float percent = (float)remaining / (float)m_dwLimitTime;
		if (percent < 0) percent = 0;
		m_pGauge = (int)(percent * 420.0f);
		SetRect(m_pSrcRect, 0, 0, 260, 446);
		m_pSpriteCom5->Render_Sprite(m_tXScale, m_tYScale, m_pSrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe1.png");
	
		m_pSpriteCom6->Render_Sprite(m_tXScale, m_tYScale, m_pSrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_1.png");

		m_pSpriteCom7->Render_Sprite(m_tXScale, m_tYScale, m_pSrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
	

}


void CUi_Sashimi::Free()
{
}