/**
* @file    CPlate.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ����(Plate) ������Ʈ Ŭ����
* @details �÷��̾ ��� �̵��ϰų�, ��Ḧ ��ų�, Ư�� ��Ḧ �÷��� �� �ִ� ������Ʈ.
*/
#pragma once
#include "CInteract.h"
#include "IPlace.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CPlate : public CInteract, public IPlace
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
	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE	Get_InteractType() const override { return CInteract::PLATE; }

	// IPlace��(��) ���� ��ӵ�
	_bool			Set_Place(CGameObject* pItem, CGameObject* pPlace) override;

	// IPlace��(��) ���� ��ӵ�
	_bool			Get_CanPlace(CGameObject* pItem) override;

private:
	HRESULT			Add_Component();
	void			Add_Ingredient(const _tchar* pTag);
	HRESULT			Change_Texture(const _tchar* pComponentTag);
	const _tchar*	IngredientTypeToString(CIngredient::INGREDIENT_TYPE eType);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	vector<wstring>	m_vecMenu;
	_tchar			m_szMenu[256];

public:
	static		CPlate*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};