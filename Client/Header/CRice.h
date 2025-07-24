/**
* @file    CRice.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ��(Rice) ��� Ŭ����
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

	// CInteract��(��) ���� ��ӵ�
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