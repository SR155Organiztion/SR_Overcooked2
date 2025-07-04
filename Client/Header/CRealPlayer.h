#pragma once
#include "CGameObject.h"
#include "iPhysics.h"
#include "CPlayerHand.h"

#include "CPlayerState.h"
#include "Player_Define.h"
#include "CInteractMgr.h"
#include "IProcess.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
	class CFSMComponent;
}


class CRealPlayer :
	public Engine::CGameObject,
	public IPhysics

{
private:
	explicit CRealPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRealPlayer(const CGameObject& rhs);
	virtual ~CRealPlayer();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	/**
	* @brief 플레이어가 1P인지 2P인지 반환하는 함수
	* @return 자료형 PLAYER_NUM을 리턴. 1P일 땐 PLAYER_1P, 2P일 땐 PLAYER_2P를 반환
	*/
	PLAYER_NUM	Get_PlayerNum() { return m_ePlayerNum; }
	/**
	* @brief 플레이어 1P와 2P를 정하는 함수
	* @param eNewPlayer - 1P일 땐 PLAYER_1P, 2P일 땐 PLAYER_2P
	*/
	void		Set_PlayerNum(PLAYER_NUM eNewPlayer) { m_ePlayerNum = eNewPlayer;; }
	CPlayerHand* Get_Hand(HAND_ID eID) { return m_vecHands[eID]; }
	/**
	* @brief 플레이어의 특수행동을 탈출하는 함수
	* @param eID - 특수행동ID(ACT_CHOP, ACT_WASH)
	* @param IsPause - 특수행동을 진행 중에 종료하는지
	* @param PlayerState - 다음 플레이어의 상태. default = Player_Idle
	*/
	void				Escape_Act(ACT_ID eID, _bool IsPause, std::string PlayerState = "Player_Idle");
	void				Change_PlayerState(std::string PlayerState);
private:
	HRESULT				Add_Component(); /// 컴포넌트 넣는거
	HRESULT				Ready_Hands();
	CGameObject*		Find_Cursor_Carriable(list<CGameObject*> listCarry);
	CGameObject*		Find_Cursor_Station(list<CGameObject*> listStation);
	void				Set_GrabObjMat();
	void				Set_HandGrab_Off();
	void				Change_HandState(std::string newState);
	void				KeyInput();

	PLAYER_NUM	m_ePlayerNum;
	vector<CPlayerHand*>	m_vecHands;
	CGameObject* m_pCursorCarriable;
	CGameObject* m_pCursorStation;
	CGameObject* m_pGrabObj;

	_int	test[3]{};
	_bool	m_bKeyCheck[256];
	_bool   m_bAct[ACT_END];
	
	IChop*	m_pIChop;
	//IWash* m_pIWash;

	_bool	Test_Carriable = false;
	_bool	Test_Station = false;


private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CFSMComponent* m_pFSMCom;
	//Engine::CAniMat* m_pAniMatCom;

public:
	static CRealPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};
