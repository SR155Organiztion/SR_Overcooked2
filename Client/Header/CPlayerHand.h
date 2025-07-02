#pragma once
#include "CGameObject.h"
#include "Player_Define.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CAnimat;
	class CTexture;
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
	void		Set_LocalMatrix(HAND_ID newHand);
	/**
	* @brief �÷��̾��� Transform ������Ʈ ��ü�� �ѱ�� �Լ�
	* @param pPlayerTransformCom - �÷��̾��� m_pTransformCom ����
	*/
	void		Set_PlayerTransformCom(Engine::CTransform* pPlayerTransformCom) {
		m_pPlayerTransformCom = pPlayerTransformCom;
	}

private:
	HRESULT		Add_Component(); 

	Engine::CCubeTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	//Engine::CTransform* m_pTransformCom;

	_matrix			m_matLocalHand;
	_matrix			m_matWorldHand;
	_float			m_fAngle;

	HAND_ID			m_eHand;
	Engine::CTransform*		m_pPlayerTransformCom;


public:
	static CPlayerHand* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual		void	Free();
};

