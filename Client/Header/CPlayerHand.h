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
	* @brief 손을 정하고 그에 따른 로컬 좌표 생성 함수
	* @param newHand - HAND_LEFT, HAND_RIGHT로 왼,오른손 구분
	*/
	void		Init_Hand(HAND_ID newHand);
	_matrix		Get_LocalMatrix() { return m_matLocalHand; }
	/**
	* @brief 플레이어의 Transform 컴포넌트 객체를 넘기는 함수
	* @param pPlayerTransformCom - 플레이어의 m_pTransformCom 전달
	*/
	void		Set_PlayerComponent(CGameObject* pPlayer, CTransform* pTransCom, CFSMComponent* pFSMCom);
	CGameObject* Get_Player() { return m_pOwner; }

	Engine::CTransform* Get_PlayerTransformCom() { return m_pPlayerTransformCom; }
	Engine::CFSMComponent* Get_PlayerFSMCom() { return m_pPlayerFMSMCom; }
	Engine::CFSMComponent* Get_HandFSMCom() { return m_pFSMCom; }
	REVINFO*	Get_RevInfo() { return m_tRevInfo; }
	/**
	* @brief 로컬 행렬를 변환할 경우에만 사용
	* - 호출 시, m_bRedefine = true로 변환
	* - 원복시 꼭 Set_bRedefine 호출할 것
	* @param changeMat - 변경된 행렬
	*/
	void		Set_Redefine_LocalMat(_matrix changeMat);
	void		Set_bRedefine(_bool Is) { m_bRedefine = Is; }

private:
	HRESULT		Add_Component(); 
	void		Set_HandWorldMat();

	Engine::CCubeTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CFSMComponent* m_pFSMCom;
	//Engine::CTransform* m_pTransformCom;

	_matrix			m_matLocalHand;
	_matrix			m_matRedefinedLocalHand;
	_matrix			m_matWorldHand;
	REVINFO*		m_tRevInfo;

	_bool			m_bRedefine;
	HAND_ID			m_eHand;
	Engine::CTransform*			m_pPlayerTransformCom;
	Engine::CFSMComponent*		m_pPlayerFMSMCom;
	CGameObject*	m_pOwner;


public:
	static CPlayerHand* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual		void	Free();
};

