#pragma once
#include "CGameObject.h"
#include "Player_Define.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	//class CAnimat;
	class CTexture;
	class CFSMComponent;
}



class CPlayerHand : public Engine::CGameObject
{
private:	
	explicit CPlayerHand(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayerHand(const CGameObject& rhs);
	virtual ~CPlayerHand();

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta) override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual			void		Render_GameObject() override;

public:
	/**
	* @brief ���� ���ϰ� �׿� ���� ���� ��ǥ ���� �Լ�
	* @param newHand - HAND_LEFT, HAND_RIGHT�� ��,������ ����
	*/
	void		Init_Hand(HAND_ID newHand);
	_matrix		Get_LocalMatrix() { return m_matLocalHand; }
	/**
	* @brief �÷��̾��� Transform ������Ʈ ��ü�� �ѱ�� �Լ�
	* @param pPlayerTransformCom - �÷��̾��� m_pTransformCom ����
	*/
	void		Set_PlayerComponent(CTransform* pTransCom, CFSMComponent* pFSMCom);

	Engine::CTransform* Get_PlayerTransformCom() { return m_pPlayerTransformCom; }
	Engine::CFSMComponent* Get_PlayerFSMCom() { return m_pPlayerFMSMCom; }
	Engine::CFSMComponent* Get_HandFSMCom() { return m_pFSMCom; }
	REVINFO* Get_RevInfo() { return m_tRevInfo; }


private:
	HRESULT		Add_Component(); 
	void		Set_HandWorldMat();

	Engine::CCubeTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CFSMComponent* m_pFSMCom;
	//Engine::CTransform* m_pTransformCom;

	_matrix			m_matLocalHand;
	_matrix			m_matWorldHand;
	REVINFO*		m_tRevInfo;

	HAND_ID			m_eHand;
	Engine::CTransform*			m_pPlayerTransformCom;
	Engine::CFSMComponent*		m_pPlayerFMSMCom;



public:
	static CPlayerHand* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual		void	Free();
};

