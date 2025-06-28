#pragma once
#include "CIngredient.h"
#include "IPlace.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CEmptyStation : public CInteract, public IPlace
{
protected:
	explicit CEmptyStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEmptyStation(const CGameObject& rhs);
	virtual ~CEmptyStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	// IPlace을(를) 통해 상속됨
	_bool Get_CanPlace(ICarry* pCarry) const override;
	void Set_CarryTypes() override;

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CEmptyStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};