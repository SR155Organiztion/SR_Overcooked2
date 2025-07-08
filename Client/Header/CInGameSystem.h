#pragma once
#include "CRecipeMgr.h"
#include "CLayer.h"
#include "CMapTool.h"
#include "CTransform.h"

class CInGameSystem
{
	DECLARE_SINGLETON(CInGameSystem)

private:
	explicit CInGameSystem();
	virtual ~CInGameSystem();

	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;

	// 주문서 총 주문서 큐
	queue<CRecipeMgr::RECIPE> m_qTotalOrderRecipe;
	// 현재 접수된 주문서
	queue<CRecipeMgr::RECIPE> m_qCurrOrderRecipe;
	
	// 점수
	_int m_iScore = 0;
	// 제한 시간
	_float m_fTimeList = 0.f;
	S_STAGE m_stCurrStageInfo;

public:
	HRESULT Ready_CInGameSystem(string _szCurrStage, LPDIRECT3DDEVICE9 _pGraphicDev);
	// 내보낸 음식과 주문서 비교
	_bool Compare_FoodRecipe(set<string> _FoodSet, set<string> _RecipeSet);
	HRESULT Parse_GameObjectData(CLayer* _pLayer);
private:
	void Parse_Direction(CTransform* _pTrans, string _szDir);
	template<typename T>
	void		Parse_Position(S_BLOCK _stBlock, CGameObject** _pGameObject);
	template<typename T>
	void		Parse_Position(S_TILE _stTile, CGameObject** _pGameObject);;

private:
	void Free();
};

