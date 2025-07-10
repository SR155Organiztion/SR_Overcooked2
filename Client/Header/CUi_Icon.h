#pragma once
#include "CUi.h"
#include "CIngredient.h"
class CUi_Icon : public CUi
{
private:
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CVIBuffer* m_pBufferCom;

	CIngredient::INGREDIENT_TYPE m_eType;

public:
	CUi_Icon();
	CUi_Icon(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Icon(const CGameObject& rhs);
	~CUi_Icon();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject();
	void Render_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	HRESULT Add_Component();
	void Make_Icon(CIngredient::INGREDIENT_TYPE _m_eType);

private:
	void Free();

};

