#pragma once
#include "CUi.h"
class CUi_PostCard : public CUi
{
private:
	Engine::CSprite* m_pSpriteCom;

public:
	CUi_PostCard();
	CUi_PostCard(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_PostCard(const CUi_PostCard& rhs);
	~CUi_PostCard();

	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev);
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject(const _float& _fTimeDelta);
	void Render_GameObject();
	HRESULT Add_Component();
	void Make_PostCard(bool _bVisible); 

	void On_Off(bool _bProcess)
	{
		m_tData.m_bProcess = _bProcess;
	}


private:
	void Free();

};

