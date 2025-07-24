#pragma once
#include "CRecipeMgr.h"
#include "CLayer.h"
#include "CMapTool.h"
#include "CTransform.h"
#include "CScene.h"
#include <CUi_Order.h>
#include <CUi_StarScore.h>

class CInGameSystem
{
	DECLARE_SINGLETON(CInGameSystem)

private:
	explicit CInGameSystem();
	virtual ~CInGameSystem();

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;

	// �ֹ��� �� �ֹ��� ť
	queue<CRecipeMgr::RECIPE> m_qTotalOrderRecipe;
	// ���� ������ �ֹ���
	// queue<CRecipeMgr::RECIPE> m_qCurrOrderRecipe;
	list<CUi_Order::ORDER>* m_pCurrOrderRecipeList;
	// ������ �ֹ���
	//queue<CRecipeMgr::RECIPE> m_qCompleteOrderRecipe;
	CRecipeMgr::RECIPE m_stCompleteOrder;

	// ����
	_int m_iScore = 0;
	_int m_iSuccessScore = 0;
	_int m_iFailScore = 0;
	_int m_iSuccessCnt = 0;
	_int m_iFailCnt = 0;

	_int m_iOneStarScore = 50;
	_int m_iTwoStarScore = 100;
	_int m_iThreeStarScore = 200;

	_int m_iReadyCnt = 0;
	_int m_iParseCnt = 0;

	// ���� �ð�
	_float m_fTimeLimit = 0.f;
	S_STAGE m_stCurrStageInfo;

	_float m_fOrderTImeInterval = 15.f;
	_float m_fOrderTimeElapsed = m_fOrderTImeInterval - 13.f;

public:
	HRESULT Ready_CInGameSystem(string _szCurrStage, LPDIRECT3DDEVICE9 _pGraphicDev, CScene* _pScene);
	_int	Update_InGameSystem(const _float& fTimeDelta, CScene* _pScene);
	void	Set_CompleteOrder(set<wstring>*	_pSetIngredient) {
		m_stCompleteOrder.setIngredient = *_pSetIngredient;
	}
public:
	HRESULT Parse_GameObjectData(CLayer* _pLayer);
	HRESULT Parse_EnviromentData(CLayer* _pLayer);
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

	void	Push_InOrder(CScene* _pScene);
	_int	Culc_Star(string _szCurrStage, CUi_StarScore* _pStarScore);

	_int Get_NumberEndOfString(string _szKey)
	{
		_tchar chNumber = _szKey.at(_szKey.size() - 1);
		_int iNumber = chNumber - '0';

		return iNumber;
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

	S_EVENT Get_Event() const {
		return m_stCurrStageInfo.Event;
	}

	_float Get_TotalTime() const {
		return m_stCurrStageInfo.Time;
	}

	S_MAPSIZE Get_MapSize() const {
		return m_stCurrStageInfo.MapSize;
	}

	void Clear();

private:
	_int Compare_FoodRecipe();
	void Parse_Direction(CTransform* _pTrans, string _szDir);
	void Parse_Direction(CTransform* _pTrans, float _fAngle);
	
	template<typename T>
	void		Parse_Position(S_BLOCK _stBlock, CGameObject** _pGameObject);
	
	template<typename T>
	void		Parse_Position(S_TILE _stTile, CGameObject** _pGameObject);

	template<typename T>
	void		Parse_Position(S_ENVOBJECT _stEnv, CGameObject** _pGameObject);

	template<typename T>
	void		Parse_Scale(S_ENVOBJECT _stEnv, CGameObject** _pGameObject);

private:
	void Free();
};

