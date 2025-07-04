#pragma once

#include "CScene.h"
#include "CMapTool.h"
#include "CTransform.h"


class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage();

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
	template<typename T>
	void		Parse_Position(S_BLOCK _stBlock, CGameObject** _pGameObject);
	HRESULT		Ready_Prototype();
	HRESULT		Ready_Light();
	HRESULT		Parse_Json(CLayer* _pLayer);
	void		Parse_Direction(CTransform* _pTrans, string _szDir);

public:
	static CStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void	Free();


};

