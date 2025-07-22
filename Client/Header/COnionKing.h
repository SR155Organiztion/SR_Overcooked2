#pragma once
#include "CNPC.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class COnionKing : public CNPC
{
public:
	enum ONION_STATE {
		ONION_DANCE,
		ONION_STAND,
		ONION_HANDSUP,
		ONION_HAPPY,
		ONION_SAD,
		ONION_END
	};

private:
	explicit COnionKing(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~COnionKing();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	void			Set_State(ONION_STATE eState) { m_eCurState = eState; }
	void			Set_Active(_bool Is) { m_bActive = Is; }

private:
	HRESULT			Add_Component();
	void			Key_Input();
	void			Walk_Onion(const _float& dt);
	void			Apply_State();
	void			Set_Billboard();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	ONION_STATE m_eCurState = ONION_DANCE;
	ONION_STATE m_ePreState = ONION_END;

	_float		m_fSpeed = 1.f;

	_float		m_fFrame{}, m_fTimeStack{}, m_fMoveDistance{};
	_float		m_fAppearTime = 5.f;
	_float		m_fScenarioTime = 10.f;
	_int		m_iFrameStack{}, m_iFrame{}, m_iAnimationStack{};
	_bool		m_bActive{}, m_bWalk{}, m_bScenarioEnd{};
	_bool		m_bTestKey{};


public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

