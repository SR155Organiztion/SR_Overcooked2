#pragma once
#include "CIngredient.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CPasta : public CIngredient
{
protected:
	explicit CPasta(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPasta(const CGameObject& rhs);
	virtual ~CPasta();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

private:
	HRESULT		Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CPasta* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};