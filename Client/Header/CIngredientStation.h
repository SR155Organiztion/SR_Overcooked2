/**
* @file    CIngredientStation.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ��� ���� ������Ʈ Ŭ����
* @details �÷��̾ ��Ḧ �������ų� ���� �� �ִ� ������ �����ϴ� �۾��� ������Ʈ.
*/
#pragma once
#include "CInteract.h"
#include "IPlace.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CIngredientStation : public CInteract, public IPlace
{
protected:
	explicit CIngredientStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CIngredientStation(const CGameObject& rhs);
	virtual ~CIngredientStation();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE			Get_InteractType() const override { return CInteract::STATION; }

	// IPlace��(��) ���� ��ӵ�
	_bool					Get_CanPlace(CGameObject* pItem) override;

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CIngredientStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free(); 
};