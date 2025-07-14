#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTileTex;
	class CTransform;
	class CTexture;
}

class CPink44Tile : public Engine::CGameObject
{
private:
	explicit CPink44Tile(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPink44Tile(const CGameObject& rhs);
	virtual ~CPink44Tile();

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
	static CPink44Tile* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

