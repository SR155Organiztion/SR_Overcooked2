#include "pch.h"
#include "CUi_Gauge.h"

CUi_Gauge::CUi_Gauge(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUi(pGraphicDev) , m_pSrcRect(nullptr)
{
	m_pSrcRect = new RECT;
	SetRect(m_pSrcRect, 0, 0, 280, 120);
}

CUi_Gauge::CUi_Gauge(const CGameObject& rhs):CUi(rhs), m_pSrcRect(nullptr)
{
	m_pSrcRect = new RECT;
	SetRect(m_pSrcRect, 0, 0, 280, 120);
}

CUi_Gauge::~CUi_Gauge()
{
}

HRESULT CUi_Gauge::Add_Component()
{
	return S_OK;
}