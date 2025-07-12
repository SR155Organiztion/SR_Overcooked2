#pragma once
#include "Engine_Define.h"
#include "CLayer.h"
#include "CMapTool.h"
#include "CScene.h"

class CSelectGameSystem
{
	DECLARE_SINGLETON(CSelectGameSystem)

private:
	explicit CSelectGameSystem();
	virtual ~CSelectGameSystem();

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;
	S_STAGE m_stCurrStageInfo;
	const string m_szCurrStage = "SelectMap";

private:
	HRESULT Parse_TileObjectData(CLayer* _pLayer, vector<S_TILE>* _pVecTile);

public:
	HRESULT Ready_CSelectGameSystem(string _szCurrStage, LPDIRECT3DDEVICE9 _pGraphicDev, CScene* _pScene);
	HRESULT Parse_GameObjectData(CLayer* _pLayer);

private:
	template<typename T>
	void		Parse_Position(S_BLOCK _stBlock, CGameObject** _pGameObject);

	template<typename T>
	void		Parse_Position(S_TILE _stTile, CGameObject** _pGameObject);
};

