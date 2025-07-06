#pragma once
#include "CScene.h"
#include "CLoading.h"

class CSelectLoading: public CScene
{
private:
    explicit CSelectLoading(LPDIRECT3DDEVICE9 pGraphicDev);
public:
    virtual ~CSelectLoading();

private:
	CLoading* m_pLoading;

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
	static CSelectLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void	Free();

};

