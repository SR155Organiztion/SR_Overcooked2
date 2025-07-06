#pragma once
#include "CScene.h"
#include "CLoading.h"

class CStageLoading :public CScene
{
private:
	explicit CStageLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStageLoading(LPDIRECT3DDEVICE9 pGraphicDev, string _szStageKey);
public:
	virtual ~CStageLoading();

private:
	CLoading*			m_pLoading;
	string		m_szStageKey;

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
	static CStageLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev, string _szStageKey);
private:
	virtual void	Free();

};

