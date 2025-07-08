#include "pch.h"
#include "CUi_OrderMgr.h"
#include "CUi_Order.h"

#include "CProtoMgr.h"

IMPLEMENT_SINGLETON(CUi_OrderMgr)

CUi_OrderMgr::CUi_OrderMgr( )
{
}

CUi_OrderMgr::CUi_OrderMgr(LPDIRECT3DDEVICE9 pGraphicDev)
{
}

CUi_OrderMgr::~CUi_OrderMgr()
{
}

HRESULT CUi_OrderMgr::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, OBJECT_TYPE _type)
{
	Add_Order(_m_pGraphicDev, _type);

	for (auto& pRecipy : m_mapOrder)
		pRecipy.second->Ready_GameObject(_m_pGraphicDev, _type);

	return S_OK;
}
int CUi_OrderMgr::Update_GameObject(const _float& _fTimeDelta)
{
	for (auto& pRecipy : m_mapOrder) {
		pRecipy.second->Update_GameObject(_fTimeDelta);
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, pRecipy.second);
	}
	return 0;
}
void CUi_OrderMgr::LateUpdate_GameObject(const _float& _fTimeDelta)
{
	for (auto& pRecipy : m_mapOrder) {
		CUi_Order* pOrder = pRecipy.second;
		pOrder->LateUpdate_GameObject(_fTimeDelta);

		if (pOrder->Get_Time() - pOrder->Get_StartTIme() >= pOrder->Get_LimitTIme())
		{
			//타임 오버 시 해당 객체(레시피) 삭제
		}
	}
		
}
void CUi_OrderMgr::Render_GameObject()
{
	for (auto& pRecipy : m_mapOrder)
		pRecipy.second->Render_GameObject();
}

HRESULT CUi_OrderMgr::Add_Order(LPDIRECT3DDEVICE9 pGraphicDev, OBJECT_TYPE _type)
{
	CUi_Order* pOrder = new CUi_Sashimi(pGraphicDev);
	pOrder->Ready_GameObject(pGraphicDev, _type);
	m_mapOrder[L"Sashimi"] = pOrder;
	return S_OK;
}

void CUi_OrderMgr::Remove_Order(wstring& key)
{
	auto it = m_mapOrder.find(key);
	if (it != m_mapOrder.end())
	{
		delete it->second;
		m_mapOrder.erase(it);
	}
}
