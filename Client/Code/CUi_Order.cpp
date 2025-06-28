#include "pch.h"
#include "CUi_Order.h"

CUi_Order::CUi_Order(LPDIRECT3DDEVICE9 pGraphicDev):CUi_Object(pGraphicDev)
{

}

CUi_Order::CUi_Order(const CGameObject& rhs): CUi_Object(rhs)
{

}

CUi_Order::~CUi_Order()
{
}

void CUi_Order::Add_Recipe(_tchar* m_pName)
{
}
