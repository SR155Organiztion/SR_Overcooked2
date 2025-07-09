#pragma once
#include "CRecipeMgr.h"
#include "CLayer.h"
#include "CMapTool.h"
#include "CTransform.h"
#include "CScene.h"

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
	queue<CRecipeMgr::RECIPE> m_qCurrOrderRecipe;
	// ������ �ֹ���
	//queue<CRecipeMgr::RECIPE> m_qCompleteOrderRecipe;
	CRecipeMgr::RECIPE m_qCompleteOrder;

	// ����
	_int m_iScore = 0;
	// ���� �ð�
	_float m_fTimeLimit = 0.f;
	S_STAGE m_stCurrStageInfo;

	_float m_fOrderTimeElapsed = 0.f;
	_float m_fOrderTImeInterval = 1.f;
public:
	HRESULT Ready_CInGameSystem(string _szCurrStage, LPDIRECT3DDEVICE9 _pGraphicDev);
	_int	Update_InGameSystem(const _float& fTimeDelta, CScene* _pScene);
public:
	// ������ ���İ� �ֹ��� ��
	HRESULT Parse_GameObjectData(CLayer* _pLayer);
	void	Setting_LimitTime(
		CGameObject* _pGameObject1
		, CGameObject* _pGameObject2
		, CGameObject* _pGameObject3
		);
	void	Setting_Score(CScene* _pScene, _int _iScore);
	void	Take_Order(CGameObject* _pGameObject);

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

