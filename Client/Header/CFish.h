/**
* @file    CFish.h
* @date    2025-06-29
* @author  권예지
* @brief   생선(Fish) 재료 클래스
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

class CFish : public CIngredient
{
protected:
	explicit CFish(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFish(const CGameObject& rhs);
	virtual ~CFish();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

private:
	HRESULT		Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CFish* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};