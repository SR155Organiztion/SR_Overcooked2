#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTileTex;
	class CTransform;
	class CTexture;
}

class CPondTile : public Engine::CGameObject
{
private:
	explicit CPondTile(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPondTile(const CGameObject& rhs);
	virtual ~CPondTile();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT		Add_Component();
	HRESULT		Set_Metarial();

private:
	Engine::CRcTileTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static		CPondTile*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

