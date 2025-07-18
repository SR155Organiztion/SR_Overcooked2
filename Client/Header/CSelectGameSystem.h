#pragma once
#include "Engine_Define.h"
#include "CLayer.h"
#include "CMapTool.h"
#include "CScene.h"
#include "CHexTile.h"
#include "CFlag.h"

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

	vector<CHexTile*> m_hexTileVec;
	const _float m_fRadius = 1.f;

	vector<CFlag*> m_flagVec;

private:
	HRESULT Parse_TileObjectData(CLayer* _pLayer, vector<S_TILE>* _pVecTile);
	HRESULT Parse_FlagData(CLayer* _pLayer, vector<S_ENVOBJECT>* _pVecTile);

public:
	HRESULT Parse_GameObjectData(CLayer* _pLayer);
	HRESULT Parse_EnviromentData(CLayer* _pLayer);
	HRESULT Parse_EnviromentObjectData(CLayer* _pLayer);
	HRESULT Ready_CSelectGameSystem(string _szCurrStage, LPDIRECT3DDEVICE9 _pGraphicDev, CScene* _pScene);
	void Find_By_Euclidean(_vec3* _vCenterPos);

	CFlag* Get_FlagByStageNum(_uint _iStageNum);

private:
	_int Get_NumberEndOfString(string _szKey);

	template<typename T>
	void		Parse_Position(S_BLOCK _stBlock, CGameObject** _pGameObject);

	template<typename T>
	void		Parse_Position(S_TILE _stTile, CGameObject** _pGameObject);

	template<typename T>
	void		Parse_Position(S_ENVOBJECT _stEnv, CGameObject** _pGameObject);

};

