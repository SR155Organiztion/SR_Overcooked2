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
	* @brief 플레이어 1P와 2P를 정하는 함수
	* @param eNewPlayer - 1P일 땐 PLAYER_1P, 2P일 땐 PLAYER_2P
	*/
	void		Set_PlayerNum(PLAYER_NUM eNewPlayer) { m_ePlayerNum = eNewPlayer; }
	/**
	* @brief 플레이어가 1P인지 2P인지 반환하는 함수
	* @return 자료형 PLAYER_NUM을 리턴. 1P일 땐 PLAYER_1P, 2P일 땐 PLAYER_2P를 반환
	*/
	PLAYER_NUM	Get_PlayerNum() { return m_ePlayerNum; }
	/**
	* @brief 플레이어의 손 포인터를 호출하는 함수
	* @param eID - HandHAND_LEFT or HAND_RIGHT
	* @return CPlayerHand* 반환 
	*/
	CPlayerHand* Get_Hand(HAND_ID eID) { return m_vecHands[eID]; }
	/**
	* @brief 플레이어의 특수행동을 탈출하는 함수
	* @param eID - 특수행동ID(ACT_CHOP, ACT_WASH)
	* @param IsPause - 특수행동을 진행 중에 종료하는지
	* @param PlayerState - 다음 플레이어의 상태. default = Player_Idle
	*/
	void				Escape_Act(ACT_ID eID, _bool IsPause, std::string PlayerState = "Player_Idle");
	/**
	* @brief 플레이어가 특수 행동(설거지, 썰기)을 하고 있는지를 반환하는 함수
	* @param eID - ACT_CHOP, ACT_WASH
	* @return true or false
	*/
	_bool				Get_Act(ACT_ID eID) { return m_bAct[eID]; }
	void				Change_PlayerState(std::string PlayerState);
	CGameObject*		Get_CursorStation();
	void				On_Detected(CGameObject* _pGameObject) override;
	void				On_Collision(CGameObject* _pGameObject) override;
	void				On_LookHit(CGameObject* _pGameObject) override;

	void				Set_PlayerFirstPos(_float x, _float y, _float z);

private:
	HRESULT				Add_Component(); /// 컴포넌트 넣는거
	HRESULT				Ready_Hands();
	CGameObject*		Find_Cursor(CURSOR_ID eID);
	void				Update_Hands(const _float dt);
	void				Set_Cursor();
	void				Set_GrabObjMat();
	void				Drop_GrabObject();
	void				Change_HandState(std::string newState);
	void				KeyInput();
	void				Reset_Cursor();
	void				Reset_DetectedList();
	void				Check_Act(const _float& dt);
	void				Shine_Cursor(CGameObject* pCursor); 
	void				GrabKey_Algorithm();
	void				ActKey_Algorithm();

	PLAYER_NUM	m_ePlayerNum;
	vector<CPlayerHand*>	m_vecHands;
	list<CGameObject*>		m_listDetected[CURSOR_END];
	CGameObject* m_pCursorCarriable;
	CGameObject* m_pCursorStation;
	CGameObject* m_pGrabObj;
	CGameObject* m_pActStation;
	

	_bool	m_bKeyCheck[256];
	_bool   m_bAct[ACT_END];
	
	IChop*	m_pIChop;
	IWash*	m_pIWash;

	
private: // For Test
	_float	test[3];
	_tchar	m_szShowTestTime[64];
	std::wstring m_strCurName[CURSOR_END];
	_bool	Test_Carriable = false;
	_bool	Test_Station = false;
	_bool	m_bTestAct[ACT_END];
 
	void	Check_CursorName();
	void	Render_TestName();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CFSMComponent* m_pFSMCom;

public:
	/**
	* @brief Player 생성 함수
	* @details 생성 후 Set_PlayerNum 호출해서 1P 2P 정할 것
	* - 플레이어 초기 생성 위치 -> Transform 컴포넌트로 정할 것
	*/
	static CRealPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};
