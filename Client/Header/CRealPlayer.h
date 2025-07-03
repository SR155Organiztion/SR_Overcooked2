#pragma once
#include "CGameObject.h"
#include "iPhysics.h"
#include "CPlayerHand.h"

#include "CPlayerState.h"
#include "Player_Define.h"
#include "CInteractMgr.h"

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

private:
	HRESULT				Add_Component(); /// 컴포넌트 넣는거
	HRESULT				Ready_Hands();
	CGameObject*		Find_Cursor_Carriable(list<CGameObject*> m_listIteract);
	CGameObject*		Find_Cursor_CStation(list<CGameObject*> m_listIteract);
	void				Set_GrabObjMat();
	void				KeyInput();

	PLAYER_NUM	m_ePlayerNum;
	vector<CPlayerHand*>	m_vecHands;
	CGameObject*		m_pCursorCarriable;
	CGameObject*		m_pCursorStation;
	CGameObject*		m_pGrabObj;
	

	_bool	m_bGrab; 

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
