#pragma once
#include "CInteract.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CInvisibleStation : public CInteract
{
protected:
	explicit CInvisibleStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInvisibleStation(const CGameObject& rhs);
	virtual ~CInvisibleStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE	Get_InteractType() const override { return CInteract::STATION; }

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CInvisibleStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};