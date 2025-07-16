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


class CFakePlayer :
	public Engine::CGameObject,
	public IPhysics
{

private:
	explicit CFakePlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFakePlayer(const CGameObject& rhs);
	virtual ~CFakePlayer();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	/**
	* @brief �÷��̾� 1P�� 2P�� ���ϴ� �Լ�
	* @param eNewPlayer - 1P�� �� PLAYER_1P, 2P�� �� PLAYER_2P
	*/
	void		Set_PlayerNum(PLAYER_NUM eNewPlayer) { m_ePlayerNum = eNewPlayer;; }
	/**
	* @brief �÷��̾ 1P���� 2P���� ��ȯ�ϴ� �Լ�
	* @return �ڷ��� PLAYER_NUM�� ����. 1P�� �� PLAYER_1P, 2P�� �� PLAYER_2P�� ��ȯ
	*/
	PLAYER_NUM	Get_PlayerNum() { return m_ePlayerNum; }
	/**
	* @brief �÷��̾��� �� �����͸� ȣ���ϴ� �Լ�
	* @param eID - HandHAND_LEFT or HAND_RIGHT
	* @return CPlayerHand* ��ȯ
	*/
	CPlayerHand* Get_Hand(HAND_ID eID) { return m_vecHands[eID]; }
	/**
	* @brief �÷��̾��� Ư���ൿ�� Ż���ϴ� �Լ�
	* @param eID - Ư���ൿID(ACT_CHOP, ACT_WASH)
	* @param IsPause - Ư���ൿ�� ���� �߿� �����ϴ���
	* @param PlayerState - ���� �÷��̾��� ����. default = Player_Idle
	*/
	void				Escape_Act(ACT_ID eID, _bool IsPause, std::string PlayerState = "Player_Idle");
	/**
	* @brief �÷��̾ Ư�� �ൿ(������, ���)�� �ϰ� �ִ����� ��ȯ�ϴ� �Լ�
	* @param eID - ACT_CHOP, ACT_WASH
	* @return true or false
	*/
	_bool				Get_Act(ACT_ID eID) { return m_bAct[eID]; }
	void				Change_PlayerState(std::string PlayerState);
	CGameObject* Get_CursorStation();
	void				On_Detected(CGameObject* _pGameObject) override;
	void				On_Collision(CGameObject* _pGameObject) override;

	void				Set_PlayerFirstPos(_float x, _float y, _float z);

private:
	HRESULT				Add_Component(); /// ������Ʈ �ִ°�
	HRESULT				Ready_Hands();
	CGameObject* Find_Cursor(CURSOR_ID eID);
	void				Update_Hands(const _float dt);
	void				Set_Cursor();
	void				Set_GrabObjMat();
	void				Drop_GrabObject();
	void				Change_HandState(std::string newState);
	void				KeyInput();
	void				Reset_Cursor();
	void				Reset_DetectedList();
	void				Check_Act(const _float& dt);
	void				Shine_Cursor(CGameObject* pCursor); // [�ӽ�]Ŀ���� ����Ű�� �ִ� ������Ʈ ���߱�

	PLAYER_NUM	m_ePlayerNum;
	vector<CPlayerHand*>	m_vecHands;
	list<CGameObject*>		m_listDetected[CURSOR_END];
	CGameObject* m_pCursorCarriable;
	CGameObject* m_pCursorStation;
	CGameObject* m_pGrabObj;
	CGameObject* m_pActStation;


	_bool	m_bKeyCheck[256];
	_bool   m_bAct[ACT_END];

	IChop* m_pIChop;
	IWash* m_pIWash;


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
	* @brief Player ���� �Լ�
	* @details ���� �� Set_PlayerNum ȣ���ؼ� 1P 2P ���� ��
	* - �÷��̾� �ʱ� ���� ��ġ -> Transform ������Ʈ�� ���� ��
	*/
	static CFakePlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};
