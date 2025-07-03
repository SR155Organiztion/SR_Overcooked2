/**
* @file    CChopStation.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ��� ��� �۾���(Chop Station) ������Ʈ Ŭ����
* @details �÷��̾ ��Ḧ �÷����� �� �� �ִ� �۾��� ������Ʈ.
*/
#pragma once
#include "CInteract.h"
#include "IPlace.h"
#include "IChop.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CChopStation : public CInteract, public IPlace, public IChop
{
protected:
	explicit CChopStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CChopStation(const CGameObject& rhs);
	virtual ~CChopStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// IChop��(��) ���� ��ӵ�

	/**
	* @brief �ش� ��Ḧ �� �� �ִ��� �Ǻ��ϴ� �Լ�.
	* @param pIngredient - ����� �ϴ� ��� ������.
	* @return true�� ��� ����, false�� �Ұ���.
	*/
	virtual _bool CanChop(CIngredient* pIngredient) const;
	/**
	* @brief �ش� ��Ḧ ��� ������ �����ϴ� �Լ�.
	* @param pIngredient - ����� �ϴ� ��� ������.
	* @details ���� ��� ������ �����ϸ�, ���� �����̳� ���൵ ������ ����.
	*/
	virtual void Chop(CIngredient* pIngredient);

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE	Get_InteractType() const override { return CInteract::STATION; }

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CChopStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

	// IPlace��(��) ���� ��ӵ�
	_bool Get_CanPlace(CGameObject* pItem) override;
};