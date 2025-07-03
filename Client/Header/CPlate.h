/**
* @file    CPlate.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ����(Plate) ������Ʈ Ŭ����
* @details �÷��̾ ��� �̵��ϰų�, ��Ḧ ��ų�, Ư�� ��Ḧ �÷��� �� �ִ� ������Ʈ.
*/
#pragma once
#include "CInteract.h"
#include "ICarry.h"
#include "IPlace.h"
#include "IPlate.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CPlate : public CInteract, public ICarry, public IPlace, public IPlate
{
protected:
	explicit CPlate(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlate(const CGameObject& rhs);
	virtual ~CPlate();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	// ICarry��(��) ���� ��ӵ�
	/**
	* @brief �ش� ������Ʈ�� ���� ��� �̵� ������ �������� Ȯ���ϴ� �Լ�.
	* @return �̵� �����ϸ� true, �Ұ����ϸ� false.
	*/
	_bool Get_CanCarry() const override;

	// IPlate��(��) ���� ��ӵ�
	/**
	* @brief �ش� ��Ḧ ���� �� �ִ��� �Ǻ��ϴ� �Լ�.
	* @param pIngredient - �������� ��� ������.
	* @return ���� �� ������ true, �Ұ����ϸ� false.
	*/
	_bool CanPlate(CIngredient* pIngredient) const override;
	/**
	* @brief �ش� ��Ḧ ��� ������ �����ϴ� �Լ�.
	* @param pIngredient - ���� ��� ������.
	* @details ���� ��Ḧ ���ÿ� �ø��� ������ ó��.
	*/
	void Plate(CIngredient* pIngredient) override;

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE	Get_InteractType() const override { return CInteract::PLATE; }

private:
	HRESULT		Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CPlate* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

	// IPlace��(��) ���� ��ӵ�
	_bool Get_CanPlace(CGameObject* pItem) override;
};