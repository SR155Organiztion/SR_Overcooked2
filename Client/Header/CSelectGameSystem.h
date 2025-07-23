#pragma once
#include "Engine_Define.h"
#include "CLayer.h"
#include "CMapTool.h"
#include "CScene.h"
#include "CHexTile.h"
#include "CFlag.h"

struct StageData {
	_bool bClear;
	_int iScore;
};

class CSelectGameSystem
{
	DECLARE_SINGLETON(CSelectGameSystem)
private:
	explicit CSelectGameSystem();
	virtual ~CSelectGameSystem();

private:
	const int HEX_DIRECTIONS[6][2] = {
		{+1,  0},
		{+1, -1},
		{ 0, -1},
		{-1,  0},
		{-1, +1},
		{ 0, +1}
	};

	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;
	S_STAGE m_stCurrStageInfo;
	const string m_szCurrStage = "SelectMap";

	vector<CHexTile*> m_hexTileVec;
	const _float m_fRadius = 1.f;

	vector<CFlag*> m_flagVec;
	_float m_fElapsedTime;
	const _float m_fFlipTimer = 0.1f;
	_bool m_bDoFlip = false;
	_bool m_bDoingFlip = false;

	map<_int, StageData> m_bClearStage;
	_int m_iCurrentStageNum;

	_bool m_bNeedFocus = false;

private:
	HRESULT Parse_TileObjectData(CLayer* _pLayer, vector<S_TILE>* _pVecTile);
	HRESULT Parse_FlagData(CLayer* _pLayer, vector<S_ENVOBJECT>* _pVecTile);

public:
	HRESULT Parse_GameObjectData(CLayer* _pLayer);
	HRESULT Parse_EnviromentData(CLayer* _pLayer);
	HRESULT Parse_EnviromentObjectData(CLayer* _pLayer);
	HRESULT Ready_CSelectGameSystem(string _szCurrStage, LPDIRECT3DDEVICE9 _pGraphicDev, CScene* _pScene);
	void Find_By_Euclidean(_vec3* _vCenterPos, _float fTimeDelta);
	void Do_Flip_Action();

	CFlag* Get_FlagByStageNum(_uint _iStageNum);
	const vector<CFlag*>* const Get_FlagVec() { return &m_flagVec; }
	void Clear();



	map<_int, StageData>* Get_ClearStageMap() { return &m_bClearStage; }
	void Set_CurStageNum(_uint _iStageNum) { m_iCurrentStageNum = _iStageNum; }
	_int Get_CurStageNum() { return m_iCurrentStageNum; }
	void Set_NeedFocus(_bool _bNeedFocus) { m_bNeedFocus = _bNeedFocus; }
	_bool Get_NeedFocus() { return m_bNeedFocus; }
private:
	_int Get_NumberEndOfString(string _szKey);

	template<typename T>
	void		Parse_Position(S_BLOCK _stBlock, CGameObject** _pGameObject);

	template<typename T>
	void		Parse_Position(S_TILE _stTile, CGameObject** _pGameObject);

	template<typename T>
	void		Parse_Position(S_ENVOBJECT _stEnv, CGameObject** _pGameObject);

};

