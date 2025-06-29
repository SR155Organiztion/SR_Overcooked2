/**
* @file    CFryingpan.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   �Ķ�����(Frying Pan) ������Ʈ Ŭ����
* @details �÷��̾ ��� �̵��ϰų�, ��Ḧ �����ϰų�, Ư�� ��Ḧ �÷��� �� �ִ� ������Ʈ.
*/
#pragma once
#include "CInteract.h"
#include "ICarry.h"
#include "IPlace.h"
#include "ICook.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CFryingpan : public CInteract, public ICarry, public IPlace, public ICook
{
protected:
	explicit CFryingpan(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFryingpan(const CGameObject& rhs);
	virtual ~CFryingpan();

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

	// IPlace��(��) ���� ��ӵ�
	/**
	* @brief �ش� ������ �������� �������� �� �ִ��� Ȯ���ϴ� �Լ�.
	* @param pCarry - �������� ICarry ������.
	* @return true�� �������� ����, false�� �Ұ���.
	*/
	_bool Get_CanPlace(ICarry* pCarry) const override;
	/**
	* @brief �ش� ������ ���� �� �ִ� ������ Ÿ���� �����ϴ� �Լ�.
	* @details ��� Ŭ�������� m_setCarryTypes�� ����� ICarry Ÿ���� ���� �߰�.
	*/
	void Set_CarryTypes() override;

	// ICook��(��) ���� ��ӵ�
	/**
	* @brief �ش� ��Ḧ ���� ������ �� �ִ��� �Ǻ��ϴ� �Լ�.
	* @param pIngredient - ���� �����Ϸ��� ��� ������.
	* @return true�� ���� ���� ����, false�� �Ұ���.
	*/
	_bool CanCook(CIngredient* pIngredient) const override;
	/**
	* @brief �ش� ��Ḧ �����ϴ� ������ �����մϴ�.
	* @param pIngredient ���� ��� ��� ������
	* @details ���� ���� ������ ó���ϸ�, ���� �����̳� ���൵ ���� ���� ���Ե� �� �ֽ��ϴ�.
	*/
	void Cook(CIngredient* pIngredient) override;

private:
	HRESULT		Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CFryingpan* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};