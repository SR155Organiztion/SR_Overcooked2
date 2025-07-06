#include "pch.h"
#include "CUi_Gauge.h"

CUi_Gauge::CUi_Gauge(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUi(pGraphicDev), m_percent(0), m_pTexBar(nullptr),m_bActivate(false), m_pTexGuage(nullptr), m_pSrcRect(nullptr)
{
	m_pSrcRect = new RECT;
	SetRect(m_pSrcRect, 0, 0, 280, 120);
}

CUi_Gauge::CUi_Gauge(const CGameObject& rhs):CUi(rhs), m_bActivate(false), m_percent(0), m_pTexBar(nullptr), m_pTexGuage(nullptr), m_pSrcRect(nullptr)
{
	m_pSrcRect = new RECT;
	SetRect(m_pSrcRect, 0, 0, 280, 120);
}

CUi_Gauge::~CUi_Gauge()
{
}
