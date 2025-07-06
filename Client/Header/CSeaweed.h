/**
* @file    CSeaweed.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ��(Seaweed) ��� Ŭ����
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

class CSeaweed : public CIngredient
{
protected:
	explicit CSeaweed(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSeaweed(const CGameObject& rhs);
	virtual ~CSeaweed();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE			Get_InteractType() const override { return CInteract::INGREDIENT; }

private:
	HRESULT					Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static		CSeaweed*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();  
};