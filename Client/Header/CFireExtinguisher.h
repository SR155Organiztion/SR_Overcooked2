#pragma once
#include "CInteract.h"
#include "ICarry.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CFireExtinguisher : public CInteract, public ICarry
{
protected:
	explicit CFireExtinguisher(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFireExtinguisher(const CGameObject& rhs);
	virtual ~CFireExtinguisher();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	// ICarry을(를) 통해 상속됨
	_bool Get_CanCarry() const override;

private:
	HRESULT		Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CFireExtinguisher* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};

