/**
* @file    CMushroom.h
* @date    2025-06-29
* @author  권예지
* @brief   버섯(Mushroom) 재료 클래스
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

class CMushroom : public CIngredient
{
protected:
	explicit CMushroom(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMushroom(const CGameObject& rhs);
	virtual ~CMushroom();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE	Get_InteractType() const override { return CInteract::INGREDIENT; }

private:
	HRESULT		Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CMushroom* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};