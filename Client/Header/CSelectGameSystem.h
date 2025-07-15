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
	unordered_map<const _tchar*, CHexTile*> m_hexTileMap;

	_vec3 m_dirVec[DIR_END] = {
		{1, -1, 0},
		{1, 0, -1},
		{0, 1, -1},
		{-1, 1, 0},
		{-1, 0, 1},
		{0, -1, 1}
	};

	vector<CHexTile*> m_willFlipHexVec[RANGE_END];

	const _float m_fRadius = 1.f;

private:
	HRESULT Parse_TileObjectData(CLayer* _pLayer, vector<S_TILE>* _pVecTile);
	void	Find_Neighbor(_vec3* _vCenterPos);
	void	Flip_Tile();

public:
	HRESULT Ready_CSelectGameSystem(string _szCurrStage, LPDIRECT3DDEVICE9 _pGraphicDev, CScene* _pScene);
	HRESULT Parse_GameObjectData(CLayer* _pLayer);

private:
	template<typename T>
	void		Parse_Position(S_BLOCK _stBlock, CGameObject** _pGameObject);

	template<typename T>
	void		Parse_Position(S_TILE _stTile, CGameObject** _pGameObject);
};

