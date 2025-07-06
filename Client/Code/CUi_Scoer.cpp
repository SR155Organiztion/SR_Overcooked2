#include "pch.h"
#include "CUi_Scoer.h"

CUi_Scoer::CUi_Scoer(): CUi_Gauge(nullptr), m_pFont(nullptr), m_pSprite(nullptr)
{
}

CUi_Scoer::CUi_Scoer(LPDIRECT3DDEVICE9 _pGraphicDev): CUi_Gauge(_pGraphicDev)
{
}

CUi_Scoer::CUi_Scoer(const CGameObject& _rhs):CUi_Gauge(_rhs)
{
}

CUi_Scoer::~CUi_Scoer()
{
}

HRESULT CUi_Scoer::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, GAUGE_TYPE _type)
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
		m_vPos = D3DXVECTOR3(1075, 650, 0);
		m_tXScale = 0.499999f;
		m_tYScale = 0.75f;

	}

	if (m_eGaugeType == LODING_GAUGE)
	{
		m_vPos = D3DXVECTOR3(1075, 650, 0);
		m_tXScale = 0.499999f;
		m_tYScale = 0.75f;
	}
}

void CUi_Scoer::Update_GameObject()
{
}

void CUi_Scoer::LateUpdate_GameObject()
{
}

void CUi_Scoer::Render_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
}

void CUi_Scoer::Free()
{
   
}