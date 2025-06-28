#include "pch.h"
#include "CUi_GameLoding.h"

CUi_GameLoding::CUi_GameLoding(LPDIRECT3DDEVICE9 pGraphicDev):CUi_Gauge(pGraphicDev)
{
}

CUi_GameLoding::CUi_GameLoding(const CGameObject& rhs):CUi_Gauge(rhs)
{
}

CUi_GameLoding::~CUi_GameLoding()
{
}

void CUi_GameLoding::Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev, float m_pPercent)
{
}

void CUi_GameLoding::Update_GameObject()
{
}

void CUi_GameLoding::LateUpdate_GameObject()
{
}

void CUi_GameLoding::Render_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
}

void CUi_GameLoding::Free()
{
}