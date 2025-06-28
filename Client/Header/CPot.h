#pragma once
#include "CInteract.h"
#include "ICarry.h"
#include "IPlace.h"
#include "ICook.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CPot : public CInteract, public ICarry, public IPlace, public ICook
{
protected:
	explicit CPot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPot(const CGameObject& rhs);
	virtual ~CPot();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	// ICarry을(를) 통해 상속됨
	_bool Get_CanCarry() const override;

	// IPlace을(를) 통해 상속됨
	_bool Get_CanPlace(ICarry* pCarry) const override;
	void Set_CarryTypes() override;

	// ICook을(를) 통해 상속됨
	_bool CanCook(CIngredient* pIngredient) const override;
	void Cook(CIngredient* pIngredient) override;

private:
	HRESULT		Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CPot* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};