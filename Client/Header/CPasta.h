/**
* @file    CPasta.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   �Ľ�Ÿ(Pasta) ��� Ŭ����
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

class CPasta : public CIngredient
{
protected:
	explicit CPasta(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPasta(const CGameObject& rhs);
	virtual ~CPasta();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE			Get_InteractType() const override { return CInteract::INGREDIENT; }

private:
	HRESULT		Add_Component();

private:

public:
	static		CPasta*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};