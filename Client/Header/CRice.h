/**
* @file    CRice.h
* @date    2025-06-29
* @author  권예지
* @brief   밥(Rice) 재료 클래스
* @details 플레이어가 들고 이동하거나 가공 가능한 재료 오브젝트.
*/
#pragma once
#include "CIngredient.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CRice : public CIngredient
{
protected:
	explicit CRice(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRice(const CGameObject& rhs);
	virtual ~CRice();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE			Get_InteractType() const override { return CInteract::INGREDIENT; }

	virtual		void		Reset() override;

private:
	HRESULT					Add_Component();
	void					Draw_BurntIcon();

private:
	CGameObject*			m_pBurntIcon = nullptr;
	_bool					m_bBurntIconVisible = false;

public:
	static		CRice*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};