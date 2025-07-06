#include "pch.h"
#include "CUi_Score.h"

/// <summary>
/// 사용법: Set또는 Get 함수로 외부에서 정보를 받아  m_iScore 변수에 넣는다.
/// </summary>

CUi_Score::CUi_Score(): CUi_Gauge(nullptr), m_pFont(nullptr), m_pSprite(nullptr), m_iScore(0)
{
}

CUi_Score::CUi_Score(LPDIRECT3DDEVICE9 _pGraphicDev): CUi_Gauge(_pGraphicDev)
{
}

CUi_Score::CUi_Score(const CGameObject& _rhs):CUi_Gauge(_rhs)
{
}

CUi_Score::~CUi_Score()
{
}

HRESULT CUi_Score::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, GAUGE_TYPE _type)
{
	m_eGaugeType = _type;

	if (FAILED(Add_Component()))
		return E_FAIL;

	if (m_eGaugeType == FONT_GAUGE)
	{
		D3DXCreateSprite(_m_pGraphicDev, &m_pSprite);
		D3DXCreateFont(
			m_pGraphicDev,         // 그래픽 디바이스
			50,                    // 글자 높이(픽셀)
			0,                     // 글자 너비(0=자동)
			FW_BOLD,               // 굵기
			1,                     // MipLevels
			FALSE,                 // Italic
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			L"Naughty Squirrel",
			&m_pFont);


	}
	if (m_eGaugeType == IMAGE_GAUGE)
	{
		m_vPos = D3DXVECTOR3(100, 650, 0);
		m_tXScale = 0.499999f;
		m_tYScale = 0.75f;
	}

	if (m_eGaugeType == LODING_GAUGE)
	{
		m_vPos = D3DXVECTOR3(100, 650, 0);
		m_tXScale = 0.499999f;
		m_tYScale = 0.75f;
	}
}

int CUi_Score::Update_GameObject(const _float& _fTimeDelta)
{
	m_iScore = 1000;
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return iExit;
}

void CUi_Score::LateUpdate_GameObject()
{
}

void CUi_Score::Render_GameObject()
{
	
	if (m_eGaugeType == FONT_GAUGE)
	{
		wchar_t szScore[32] = { 0 };
		swprintf(szScore, 32, L"%02d\n", m_iScore);
		RECT rc;
		SetRect(&rc, 0, 600-50, 800, 600); //left, top, right, bottom
		HRESULT hr = m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		m_pFont->DrawTextW(m_pSprite, szScore, -1, &rc, DT_LEFT | DT_BOTTOM, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pSprite->End();

		if (m_iScore > 1000 )
		{

			float fAlpha = abs(sin(GetTickCount64() * 0.005f)); 
			int alphaValue = (int)(fAlpha * 255);
			
			HRESULT hr = m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
			m_pFont->DrawTextW(m_pSprite, szScore, -1, &rc, DT_LEFT | DT_BOTTOM, D3DCOLOR_ARGB(alphaValue, 255, 0, 0));
			m_pSprite->End(); 

			D3DXMATRIX matIdentity;
			D3DXMatrixIdentity(&matIdentity);
			m_pSprite->SetTransform(&matIdentity);
		}
	}
}

void CUi_Score::Free()
{
   
}