#pragma once
#include "CUi.h"
class CUi_Main : public CUi
{
private:
	Engine::CSprite* m_pSpriteCom;

public:
	CUi_Main();
	CUi_Main(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Main(const CUi_Main& rhs);
	~CUi_Main();

	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev);
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject(const _float& _fTimeDelta);
	void Render_GameObject();
	HRESULT Add_Component();


private:
	void Free();

};

