#include "pch.h"
#include "CUi_Factory.h"

CUi_Factory::CUi_Factory(LPDIRECT3DDEVICE9 pGraphicDev):Engine::CGameObject(pGraphicDev)
{
}

CUi_Factory::CUi_Factory(const CGameObject& rhs):Engine::CGameObject(rhs)
{
}

CUi_Factory::~CUi_Factory()
{
}

void CUi_Factory::CUi_Creat(_tchar* _m_szName)
{
}

void CUi_Factory::Button_Creat(_tchar* _m_szName)
{
}

void CUi_Factory::Steage_creat(_tchar* _m_szName)
{
}

void CUi_Factory::Order_Creat(_tchar* _m_szName)
{
}

void CUi_Factory::Gauge_Creat(_tchar* _m_szName)
{
}

void CUi_Factory::GameEnd_Creat(_tchar* _m_szName)
{
}

void CUi_Factory::Free()
{
}