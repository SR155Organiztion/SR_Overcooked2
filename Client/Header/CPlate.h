#pragma once
#include "CInteract.h"
#include "ICarry.h"
#include "IPlace.h"
#include "IPlate.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CPlate : public CInteract, public ICarry, public IPlace, public IPlate
{
protected:
	explicit CPlate(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlate(const CGameObject& rhs);
	virtual ~CPlate();

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

	// IPlate을(를) 통해 상속됨
	_bool CanPlate(CIngredient* pIngredient) const override;
	void Plate(CIngredient* pIngredient) override;

private:
	HRESULT		Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CPlate* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};