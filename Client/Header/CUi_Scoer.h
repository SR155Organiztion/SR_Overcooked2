#pragma once
#include "CUi_Object.h"
class CUi_Scoer :public CUi_Object
{
private:
	CUi_Scoer(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Scoer(const CGameObject& rhs);
	~CUi_Scoer();

public:
	void Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	void Update_GameObject();
	void LateUpdate_GameObject();
	virtual void Render_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	void Free();
};

