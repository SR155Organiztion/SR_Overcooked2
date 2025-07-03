/**
* @file    CMushroom.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ����(Mushroom) ��� Ŭ����
* @details �÷��̾ ��� �̵��ϰų� ���� ������ ��� ������Ʈ.
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

	// CInteract��(��) ���� ��ӵ�
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