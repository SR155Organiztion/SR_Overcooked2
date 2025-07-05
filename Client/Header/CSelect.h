#pragma once
#include "CScene.h"
#include "CLoading.h"

class CSelect : public CScene
{
private:
    explicit CSelect(LPDIRECT3DDEVICE9 pGraphicDev);
public:
    virtual ~CSelect();

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
	static CSelect* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void	Free();
};

