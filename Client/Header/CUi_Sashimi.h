#pragma once
#include "CUi_Order.h"
class CUi_Sashimi : public CUi_Order
{
public:
	CUi_Sashimi() = default;
	CUi_Sashimi(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Sashimi(const CUi_Sashimi& rhs);
	~CUi_Sashimi();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, OBJECT_TYPE _type);
	int Update_GameObject(const _float& _fTimeDelta)override;
	void LateUpdate_GameObject(const _float& _fTimeDelta)override;
	void Render_GameObject()override;
	void Free()override;


};

