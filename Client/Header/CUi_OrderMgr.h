#pragma once
#include "CUi_Order.h"
#include "CUi_Sashimi.h"

class CUi_OrderMgr
{
	DECLARE_SINGLETON(CUi_OrderMgr)

private:
	CUi_Sashimi* m_pSashimi;
	map<wstring, CUi_Order*> m_mapOrder;

public:
	CUi_OrderMgr();
	CUi_OrderMgr(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_OrderMgr(const CUi_Object& rhs) = delete;
	~CUi_OrderMgr();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, OBJECT_TYPE _type);
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject(const _float& _fTimeDelta);
	void Render_GameObject();
	HRESULT Add_Order(LPDIRECT3DDEVICE9 pGraphicDev, OBJECT_TYPE _type);
	void Remove_Order(wstring& key);

protected:
	void Free();
};

