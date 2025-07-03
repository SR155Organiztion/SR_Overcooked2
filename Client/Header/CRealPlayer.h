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
	* @brief �÷��̾ 1P���� 2P���� ��ȯ�ϴ� �Լ�
	* @return �ڷ��� PLAYER_NUM�� ����. 1P�� �� PLAYER_1P, 2P�� �� PLAYER_2P�� ��ȯ
	*/
	PLAYER_NUM	Get_PlayerNum() { return m_ePlayerNum; } 
	/**
	* @brief �÷��̾� 1P�� 2P�� ���ϴ� �Լ�
	* @param eNewPlayer - 1P�� �� PLAYER_1P, 2P�� �� PLAYER_2P
	*/
	void		Set_PlayerNum(PLAYER_NUM eNewPlayer) { m_ePlayerNum = eNewPlayer;; }
	CPlayerHand* Get_Hand(HAND_ID eID) { return m_vecHands[eID]; }

private:
	HRESULT				Add_Component(); /// ������Ʈ �ִ°�
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
