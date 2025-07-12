/**
* @file    CPlate.h
* @date    2025-07-09
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
public:
	/**
	* @enum PLATESTATE
	* @brief ���� ���¸� ��Ÿ���� ������
	*/
	enum PLATESTATE { 
		CLEAN,	///< ������ �� ����
		PLATED, ///< �丮�� �ö� ����
		DIRTY	///< ������ �� ����
	};

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
	/**
	* @brief ��� ��� set �����̳� �ּҰ��� ��ȯ
	* @return ��� ��� set �����̳� �ּҰ��� ��ȯ
	*/
	set<wstring>*	Get_Ingredient(){ return &m_setIngredient; }

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE	Get_InteractType() const override { return CInteract::PLATE; }

	PLATESTATE		Get_State() const { return m_ePlateState; }

	void			Set_Dirty();

	void			Set_Clean();

	// IPlace��(��) ���� ��ӵ�
	/**
	* @brief	�÷����� ��ῡ �°� ���� �̹����� ��ü�ϰ�, ���� ������Ʈ Ǯ�� ��ȯ
	*			�̹� �÷����� ���� �ø��� ���ϰ� false�� ��ȯ
	*/
	_bool			Set_Place(CGameObject* pItem, CGameObject* pPlace) override;

	// IPlace��(��) ���� ��ӵ�
	_bool			Get_CanPlace(CGameObject* pItem) override;

private:
	HRESULT			Add_Component();
	void			Set_State(PLATESTATE ePlateState);
	_bool			Add_Ingredient(const _tchar* pTag);
	_bool			Change_Texture(const _tchar* pComponentTag);
	const _tchar*	IngredientTypeToString(CIngredient::INGREDIENT_TYPE eType);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	set<wstring>	m_setIngredient;	
	_tchar			m_szMenu[256];

	PLATESTATE		m_ePlateState = CLEAN;

public:
	static		CPlate* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};