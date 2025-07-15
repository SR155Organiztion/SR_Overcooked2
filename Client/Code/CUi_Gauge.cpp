#include "pch.h"
#include "CUi_Gauge.h"

CUi_Gauge::CUi_Gauge(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUi(pGraphicDev) 
{
	
	
}

CUi_Gauge::CUi_Gauge(const CGameObject& rhs):CUi(rhs)
{

}

CUi_Gauge::~CUi_Gauge()
{
}

HRESULT CUi_Gauge::Add_Component()
{
	return S_OK;
}