#pragma once
#include "Engine_Define.h"
#include "CLayer.h"
#include "CMapTool.h"
#include "CScene.h"
#include "CHexTile.h"

class CSelectGameSystem
{
	DECLARE_SINGLETON(CSelectGameSystem)

private:
	enum HEX_DIRECTION {
		UP
		, RIGHT_UP
		, RIGHT_DOWN
		, DOWN
		, LEFT_DOWN
		, LEFT_UP
		, DIR_END
	};

	enum WILL_FLIPED_TILE_RANGE {
		FIRST_RANGE
		, SECOND_RANGE
		, THIRD_RANGE
		, RANGE_END
	};

private:
	explicit CSelectGameSystem();
	virtual ~CSelectGameSystem();

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;
	S_STAGE m_stCurrStageInfo;
	const string m_szCurrStage = "SelectMap";
	map<pair<int, int>, CHexTile*> m_hexTileMap;

	pair<int, int> evenDirectionPair[6] = {
	{  0, -1 },  // UP
	{ +1, -1 },  // RIGHT_UP
	{ +1,  0 },  // RIGHT_DOWN
	{  0, +1 },  // DOWN
	{ -1,  0 },  // LEFT_DOWN
	{ -1, -1 }   // LEFT_UP
	};

	pair<int, int> oddDirectionPair[6] = {
		{  0, -1 },  // UP
		{ +1,  0 },  // RIGHT_UP
		{ +1, +1 },  // RIGHT_DOWN
		{  0, +1 },  // DOWN
		{ -1, +1 },  // LEFT_DOWN
		{ -1,  0 }   // LEFT_UP
	};

	vector<CHexTile*> m_willFlipHexVec[RANGE_END];

	const _float m_fRadius = 1.f;

private:
	HRESULT Parse_TileObjectData(CLayer* _pLayer, vector<S_TILE>* _pVecTile);
	HRESULT Parse_FlagData(CLayer* _pLayer, vector<S_ENVOBJECT>* _pVecTile);

public:
	HRESULT Parse_GameObjectData(CLayer* _pLayer);
	HRESULT Parse_EnviromentData(CLayer* _pLayer);
	HRESULT Ready_CSelectGameSystem(string _szCurrStage, LPDIRECT3DDEVICE9 _pGraphicDev, CScene* _pScene);
	void Find_By_Euclidean(_vec3* _vCenterPos);

private:
	template<typename T>
	void		Parse_Position(S_BLOCK _stBlock, CGameObject** _pGameObject);

	template<typename T>
	void		Parse_Position(S_TILE _stTile, CGameObject** _pGameObject);

	template<typename T>
	void		Parse_Position(S_ENVOBJECT _stEnv, CGameObject** _pGameObject);
};

