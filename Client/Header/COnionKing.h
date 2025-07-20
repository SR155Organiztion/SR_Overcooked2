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
private:
	explicit COnionKing(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~COnionKing();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Add_Component();
	void			Key_Input();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	_float		m_fFrame;
	_bool		m_bAnimation{};
	_bool		m_bTestKey{};


public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

