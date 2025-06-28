#include "pch.h"
#include "CUi_TimeLimit.h"

CUi_TimeLimit::CUi_TimeLimit(LPDIRECT3DDEVICE9 pGraphicDev): CUi_Object(pGraphicDev), m_fmainTime(0.f), m_fMaxTime(0.f), m_szTime()
{
}

CUi_TimeLimit::CUi_TimeLimit(const CGameObject& rhs):CUi_Object(rhs), m_fmainTime(0.f), m_fMaxTime(0.f), m_szTime()
{
}

CUi_TimeLimit::~CUi_TimeLimit()
{
}

void CUi_TimeLimit::Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
}

void CUi_TimeLimit::Update_GameObject()
{
}

void CUi_TimeLimit::Render_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
}