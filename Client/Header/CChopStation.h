#pragma once
#include "CInteract.h"
#include "IPlace.h"
#include "IChop.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CChopStation : public CInteract, public IPlace, public IChop
{
protected:
	explicit CChopStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CChopStation(const CGameObject& rhs);
	virtual ~CChopStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	// IPlace을(를) 통해 상속됨
	_bool Get_CanPlace(ICarry* pCarry) const override;
	void Set_CarryTypes() override;

	// IChop을(를) 통해 상속됨
	_bool CanChop(CIngredient* pIngredient) const override;
	void Chop(CIngredient* pIngredient) override;

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CChopStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};