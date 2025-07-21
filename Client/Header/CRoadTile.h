#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTileTex;
	class CTransform;
	class CTexture;
}

class CRoadTile : public Engine::CGameObject
{
private:
	explicit CRoadTile(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRoadTile(const CGameObject& rhs);
	virtual ~CRoadTile();

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
	static		CRoadTile* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

