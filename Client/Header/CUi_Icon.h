#pragma once
#include "CUi.h"
class CUi_Icon : public CUi
{
private:
	map<_tchar*,LPDIRECT3DTEXTURE9*> m_mapIcon;

	Engine::CSprite* m_pSpriteCom;
	ICON_TYPE m_eType;
	_tchar* m_szName;
	_vec3 m_pPosition;
	LPDIRECT3DTEXTURE9 m_pTexIcon; 

private:
	CUi_Icon(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Icon(const CGameObject& rhs);
	~CUi_Icon();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject();
	void Render_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	HRESULT Add_Component();

private:
	void Free();

};

