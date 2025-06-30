#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CRealPlayer : public Engine::CGameObject
{
public:
	/**
	* @struct ACT_ID
	* @brief �÷��̾��� Ư�� �ൿ�� ���� ����ü
	*/
	enum ACT_ID {
		ACT_CHOP,/// ��� ���� ��
		ACT_WASH,/// ������ �� ��
		ACT_EXTINGUISH,/// ��ȭ�� ��� ���� �� ��
		ACT_END
	};
	/**
	* @struct PLAYER_ROT
	* @brief �÷��̾��� �̵� ���⿡ ���� ����ü
	*/
	enum PLAYER_ROT {
		PLAYER_L,
		PLAYER_R,
		PLAYER_U,
		PLAYER_D,
		PLAYER_LD,
		PLAYER_RD,
		PLAYER_LU,
		PLAYER_RU,
		ROT_END
	};
	/**
	* @struct PLAYER_NUM
	* @brief �÷��̾��� ��ü�� ���� ����ü
	*/
	enum PLAYER_NUM {
		PLAYER_1P,
		PLAYER_2P,
		PLAYERNUM_END
	};

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

private:
	PLAYER_NUM	m_ePlayerNum;

	HRESULT		Add_Component(); /// ������Ʈ �ִ°�
	void		Key_Input(const _float& fTimeDelta); 
	
	void		Rotate_Player(PLAYER_ROT eDir); /// �÷��̾�
	//CInteract*		Find_Cursor_Carriable(list<CInteractable*> m_listIteract);
	//CInteract*		Find_Cursor_CStation(list<CInteractable*> m_listIteract);

	//CInteract* m_pCursorCarriable;
	//CInteract* m_pCursorStation;
	//CInteract* m_pGrabObj;
	

	bool	m_bGrab; 

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	//Engine::CCalculator* m_pCalculatorCom;
	//Engine::CAniMat* m_pAniMatCom;
	//Engine::CPhysics* m_pPhysicsCom;
	//Engine::CCollision* m_pCCollisionCom;



public:
	static CRealPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
	


private:

	class CState {
	public:
		virtual	void		Enter_State(Engine::CGameObject* Obj) = 0;
		virtual	void		Update_State(Engine::CGameObject* Obj, const _float& fTimeDelta) = 0;
		virtual	void		TestForExit_State(Engine::CGameObject* Obj) = 0;
	};

	class CPlayerIdle : public CState
	{
	public:
		virtual	void		Enter_State(Engine::CGameObject* Obj) override;
		virtual	void		Update_State(Engine::CGameObject* Obj, const _float& fTimeDelta) override;
		virtual	void		TestForExit_State(Engine::CGameObject* Obj) override;
	};
	
	class CPlayerMove : public CState
	{
	public:
		virtual	void		Enter_State(Engine::CGameObject* Obj) override;
		virtual	void		Update_State(Engine::CGameObject* Obj, const _float& fTimeDelta) override;
		virtual	void		TestForExit_State(Engine::CGameObject* Obj) override;
		PLAYER_ROT			m_eDir;
	};

	class CPlayerAct : public CState
	{
	public:
		virtual	void		Enter_State(Engine::CGameObject* Obj);
		virtual	void		Update_State(Engine::CGameObject* Obj, const _float& fTimeDelta);
		virtual	void		TestForExit_State(Engine::CGameObject* Obj);
				void		Set_Act(ACT_ID eID) { m_eCurAct = eID; }
				ACT_ID		m_eCurAct;
	};

	CState*	m_eCurState;
	CState*	m_eIdleState;
	CState*	m_eMoveState;
	CState*	m_eActState;
	


	void	Change_State(CState* eState);
	







};
