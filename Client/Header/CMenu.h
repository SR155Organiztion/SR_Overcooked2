#pragma once
#include "CScene.h"

class CMenu : public CScene
{
private:
	explicit CMenu(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CMenu();

private:
	_int m_iMapSize = 0;

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual			void		Render_Scene();

private:
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_GameObject_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_UI_Layer(const _tchar* pLayerTag);

private:
	HRESULT		Ready_Prototype();

public:
	static CMenu* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void	Free();
};

