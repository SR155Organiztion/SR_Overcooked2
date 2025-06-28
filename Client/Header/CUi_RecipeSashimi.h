#pragma once
#include "CUi_Order.h"
class CUi_RecipeSashimi : public CUi_Order
{
private:
	CUi_RecipeSashimi(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_RecipeSashimi(const CGameObject& rhs);
	~CUi_RecipeSashimi();

public:
	void Ready_Ui(LPDIRECT3DDEVICE9 m_pGraphicDev);
	void Update_Ui();
	void LateUpdate_Ui();
	void Render_Ui(LPDIRECT3DDEVICE9 m_pGraphicDev);
	void Free();
};

