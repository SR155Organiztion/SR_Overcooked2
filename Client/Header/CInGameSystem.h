#pragma once
#include "CRecipeMgr.h"
#include "CLayer.h"
#include "CMapTool.h"
#include "CTransform.h"
#include "CScene.h"
#include <CUi_Order.h>

class CInGameSystem
{
	DECLARE_SINGLETON(CInGameSystem)

private:
	explicit CInGameSystem();
	virtual ~CInGameSystem();

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;

	// 주문서 총 주문서 큐
	queue<CRecipeMgr::RECIPE> m_qTotalOrderRecipe;
	// 현재 접수된 주문서
	// queue<CRecipeMgr::RECIPE> m_qCurrOrderRecipe;
	list<CUi_Order::ORDER>* m_pCurrOrderRecipeList;
	// 접수한 주문서
	//queue<CRecipeMgr::RECIPE> m_qCompleteOrderRecipe;
	CRecipeMgr::RECIPE m_stCompleteOrder;

	// 점수
	_int m_iScore = 0;
	_int m_iSuccessScore = 0;
	_int m_iFailScore = 0;
	_int m_iSuccessCnt = 0;
	_int m_iFailCnt = 0;

	// 제한 시간
	_float m_fTimeLimit = 0.f;
	S_STAGE m_stCurrStageInfo;

	_float m_fOrderTimeElapsed = 0.f;
	_float m_fOrderTImeInterval = 3.f;

public:
	HRESULT Ready_CInGameSystem(string _szCurrStage, LPDIRECT3DDEVICE9 _pGraphicDev, CScene* _pScene);
	_int	Update_InGameSystem(const _float& fTimeDelta, CScene* _pScene);
	void	Set_CompleteOrder(set<wstring>*	_pSetIngredient) {
		m_stCompleteOrder.setIngredient = *_pSetIngredient;
	}
public:
	HRESULT Parse_GameObjectData(CLayer* _pLayer);
	HRESULT	Parse_BlockObjectData(CLayer* _pLayer, vector<S_BLOCK>* _pVecBlock);
	HRESULT Parse_TileObjectData(CLayer* _pLayer, vector<S_TILE>* _pVecTile);
	HRESULT Parse_OnStationToolData(CLayer* _pLayer, S_BLOCK* _pBlock, CGameObject* _pGameObject);
	HRESULT	Parse_ETCData(CLayer* _pLayer);
	void	Setting_LimitTime(
		CGameObject* _pGameObject1
		, CGameObject* _pGameObject2
		, CGameObject* _pGameObject3
		);

	void	Setting_Score(CScene* _pScene, _int _iScore);
	void	Take_Order(CGameObject* _pGameObject);
	void	Set_OrderList(CGameObject* _pGameObject) {
		CUi_Order* pOrder = dynamic_cast<CUi_Order*>(_pGameObject);
		m_pCurrOrderRecipeList = pOrder->Get_OrderData();
	}

	_int Get_SuccessScore() const {
		return m_iSuccessScore;
	}

	_int Get_FailScore() const {
		return m_iFailScore;
	}

	_int Get_Score() const {
		return m_iScore;
	}

private:
	_int Compare_FoodRecipe();
	void Parse_Direction(CTransform* _pTrans, string _szDir);
	
	template<typename T>
	void		Parse_Position(S_BLOCK _stBlock, CGameObject** _pGameObject);
	
	template<typename T>
	void		Parse_Position(S_TILE _stTile, CGameObject** _pGameObject);

private:
	void Free();
};

