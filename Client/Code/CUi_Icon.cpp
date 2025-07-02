#include "pch.h"
#include "CUi_Icon.h"

CUi_Icon::CUi_Icon(LPDIRECT3DDEVICE9 pGraphicDev):CUi(pGraphicDev), m_pTexIcon(nullptr), m_szName(nullptr), m_pPosition(0,0,0)
{
}

CUi_Icon::CUi_Icon(const CGameObject& rhs):CUi(rhs), m_pTexIcon(nullptr), m_szName(nullptr), m_pPosition(0,0,0)
{
}

CUi_Icon::~CUi_Icon()
{
}

void CUi_Icon::Ready_GameObject(_tchar* m_szName, _vec3 m_pPosition, LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	
}

void CUi_Icon::Update_GameObject()
{
}

void CUi_Icon::LateUpdate_GameObject()
{
}

void CUi_Icon::Render_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
}

void CUi_Icon::Free()
{
}