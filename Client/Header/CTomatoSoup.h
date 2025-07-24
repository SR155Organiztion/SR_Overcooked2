/**
* @file    CTomatoSoup.h
* @date    2025-07-11
* @author  �ǿ���
* @brief   �丶�佺��(TomatoSoup) ��� Ŭ����
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

class CTomatoSoup : public CIngredient
{
protected:
	explicit CTomatoSoup(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTomatoSoup(const CGameObject& rhs);
	virtual ~CTomatoSoup();

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
	static		CTomatoSoup* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};