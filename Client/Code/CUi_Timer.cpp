#include "pch.h"
#include "CUi_Timer.h"
#include "CUi_Button.h"
#include "CUi_TimeOut.h"

//engine
#include "CSprite.h"
#include "CManagement.h"
#include "CProtoMgr.h"

/// <summary>
/// 사용법: Set또는 Get 함수로 외부에서 정보를 받아 m_dwStartTime(시작시간), m_dwLimitTime(제한시간) 변수에 넣는다.
/// </summary>
CUi_Timer::CUi_Timer(): CUi_Gauge(nullptr),m_pFont(nullptr), m_pSprite(nullptr)
{
	
	
	
}

CUi_Timer::CUi_Timer(LPDIRECT3DDEVICE9 pGraphicDev): CUi_Gauge(pGraphicDev)
{
	
	
}

CUi_Timer::CUi_Timer(const CGameObject& rhs) :CUi_Gauge(rhs)
{
	

}

CUi_Timer::~CUi_Timer()
{
	Free();
}

HRESULT CUi_Timer::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, GAUGE_TYPE _type)
{
	
	m_eGaugeType = _type;
	
	if (FAILED(Add_Component()))
		return E_FAIL;

	if (m_eGaugeType == FONT_GAUGE)
	{
		D3DXCreateSprite(_m_pGraphicDev, &m_pSprite);
		D3DXCreateFont(
			m_pGraphicDev,         
			50,                    // 글자 높이(픽셀)
			0,                     
			FW_BOLD,               
			1,                     
			FALSE,                 
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			L"Naughty Squirrel",
			&m_pFont);


	}
	if (m_eGaugeType == IMAGE_GAUGE || m_eGaugeType == LODING_GAUGE)
	{
		m_tData.m_vPos = D3DXVECTOR3(1075,650, 0);
		m_tData.m_fXScale = 0.499999f;
		m_tData.m_fYScale = 0.75f;
	
	}

	return S_OK;
}

int CUi_Timer::Update_GameObject(const _float& _fTimeDelta)
{

	if (m_dwTime >= m_tData.m_dwLimitTime + 1)
	{
		CGameObject* pTimeOut= Engine::CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_TimeOut");
		dynamic_cast<CUi_TimeOut*>(pTimeOut)->Set_TimeOut(true);
	}
	else
	{
		m_dwTime += _fTimeDelta;
	}

	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	return iExit;
}

void CUi_Timer::LateUpdate_GameObject()
{
}

void CUi_Timer::Render_GameObject()
{

	_float a = Get_Timer();
	float remaining = m_tData.m_dwLimitTime - m_dwTime;
	m_iminute = (int)remaining / 60;
	m_iseconds = (int)remaining % 60;

	if (m_eGaugeType == FONT_GAUGE)
	{
		wchar_t szTime[32];
		swprintf(szTime, 32, L"%02d:%02d\n", m_iminute, m_iseconds +1);
		RECT rc;
		SetRect(&rc, 800 - 220, 600 - 85, 800 - 20, 600 - 45); //left, top, right, bottom
		HRESULT hr = m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		m_pFont->DrawTextW(m_pSprite, szTime, -1, &rc, DT_LEFT | DT_BOTTOM, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pSprite->End();
		
		if (m_iminute <= 0 && m_iseconds <= 15)
		{

			float fAlpha = abs(sin(GetTickCount64() * 0.005f));
			int alphaValue = (int)(fAlpha * 255);

			HRESULT hr = m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
			m_pFont->DrawTextW(m_pSprite, szTime, -1, &rc, DT_LEFT | DT_BOTTOM, D3DCOLOR_ARGB(alphaValue, 255, 0, 0));
			m_pSprite->End();

			D3DXMATRIX matIdentity;
			D3DXMatrixIdentity(&matIdentity);
			m_pSprite->SetTransform(&matIdentity);
		}
	}
	
	
	
	//이미지
	
	if (m_eGaugeType == IMAGE_GAUGE)
	{
		float percent = (float)remaining / (float)m_tData.m_dwLimitTime;
		if (percent < 0) percent = 0;
		m_pGauge = (int)(percent * 420.0f)+10;
		SetRect(m_tData.m_pSrcRect, 0, 0, m_pGauge, 120);
		m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, m_tData.m_pSrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Timer1.png");
	}

	if (m_eGaugeType == LODING_GAUGE)
	{
		m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Timer0.png");
	}
	


}

HRESULT CUi_Timer::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Timer"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });

	pComponent = m_pSpriteCom2 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Timer"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite2", pComponent });

	return S_OK;
}

void CUi_Timer::Set_Timer(DWORD _dwLimitTime)
{

	m_tData.m_dwLimitTime = _dwLimitTime;

}

void CUi_Timer::Free()
{
	if (m_tData.m_pSrcRect) {
		delete m_tData.m_pSrcRect;
		m_tData.m_pSrcRect = nullptr;
	}
}

