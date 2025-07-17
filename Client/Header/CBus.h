#pragma once
#include "CGameObject.h"
#include "Player_Define.h"

namespace Engine 
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CBus : public Engine::CGameObject
{
private:
	explicit CBus(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBus();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	void				Set_FirstPos(_float x, _float y, _float z);
	void				Set_FirstScale(_float x, _float y, _float z);

private:
	HRESULT				Add_Component(); 
	void				Key_Input();
	void				Check_CoolTime(const _float& dt);
	void				Move_Bus(const _float& dt);
	void				Rotate_Bus(const _float& dt);
	void				Dash_Bus(const _float& dt);

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	_float				m_fSpeed = 6.f;
	_float				m_fDashTime{};
	_float				m_fDashCoolTime = 0.5f;
	_bool				m_bDash{}, m_bDashCool{}, m_bCheckKey{};
	PLAYER_ROT			m_eDir;
	_float				m_fEffect{};
	_bool				m_bEffect{};

public:
	static CBus* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};

