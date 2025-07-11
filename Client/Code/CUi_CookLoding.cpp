#include "pch.h"
#include "CUi_CookLoding.h"


CUi_CookLoding::CUi_CookLoding(LPDIRECT3DDEVICE9 pGraphicDev): CUi_Gauge(pGraphicDev), m_fProgress(0.f), m_bProcess(false)
{
}

CUi_CookLoding::CUi_CookLoding(const CGameObject& rhs): CUi_Gauge(rhs), m_fProgress(0.f), m_bProcess(false)
{
}

CUi_CookLoding::~CUi_CookLoding()
{
}


HRESULT CUi_CookLoding::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{
	//m_dwStartTime = GetTickCount64();
	//m_dwLimitTime = 60000; //제한 시간 180000

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tData.m_vPos = D3DXVECTOR3(500, 500, 0);
	m_tData.m_fXScale = 0.499999f;
	m_tData.m_fYScale = 0.75f;

	
	return S_OK;
}

_int CUi_CookLoding::Update_GameObject(const _float& _fTimeDelta)
{
	m_dwTime += _fTimeDelta;

	m_iFrameCount++;
	if (m_iFrameCount % 100 == 0)
	{
		m_iGaugeFrame++;
		if (m_iGaugeFrame >= 100)
		{
			// 끝까지 갔으면 멈추기
		}
	}
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	
	return iExit;
}

void CUi_CookLoding::LateUpdate_GameObject(const _float& _fTimeDelta)
{
}

void CUi_CookLoding::Render_GameObject()
{
	m_dwTime = GetTickCount64();
	float remaining = (m_dwLimitTime > (m_dwTime - m_dwStartTime)) ? (m_dwLimitTime - (m_dwTime - m_dwStartTime)) : 0;
	m_iminute = (int)(remaining / 1000) / 60;
	m_iseconds = (int)(remaining / 1000) % 60;

	//이미지
	float percent = (float)remaining / (float)m_dwLimitTime;
	if (percent < 0) percent = 0;
	m_pGauge = (int)(percent * 420.0f) + 10;
	SetRect(m_pSrcRect, 0, 0, m_pGauge, 120);
	m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, m_pSrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Timer1.png");

	if (m_iminute <= 0 && m_iseconds <= 15)
	{
		//종료
	}
}

void CUi_CookLoding::Make_cookLoding(bool _m_bProcess, _float _m_fProgress)
{
	m_bProcess = _m_bProcess;
	m_fProgress = _m_fProgress;

	if (m_bProcess)
	{
		//정보 넣기
	}

	if (!m_bProcess)
	{
		return;
	}
}

HRESULT CUi_CookLoding::Add_Component()
{

	return S_OK;
}
HRESULT CUi_CookLoding::Delete_Component()
{
	return S_OK;
}
void CUi_CookLoding::Free()
{
}
