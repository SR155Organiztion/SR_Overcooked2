#pragma once

#include "CScene.h"
#include "CMapTool.h"
#include "CTransform.h"


class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev, string _szCurrStage);
	virtual ~CStage();

private:
	string m_szCurrStage = "";
	S_STAGE m_stCurrStageInfo;
	

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
	HRESULT		Ready_Light();

public:
	static CStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CStage* Create(LPDIRECT3DDEVICE9 pGraphicDev, string _szStageKey);
private:
	virtual void	Free();


};

