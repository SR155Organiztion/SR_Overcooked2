/**
* @file    CPlate.h
* @date    2025-07-09
* @author  권예지
* @brief   접시(Plate) 오브젝트 클래스
* @details 플레이어가 들고 이동하거나, 재료를 담거나, 특정 재료를 올려둘 수 있는 오브젝트.
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
	void			Clear_Plate();
	/**
	* @brief 재료 목록을 반환합니다. (읽기 전용)
	* @return const 참조 형태로 반환되는 재료 목록
	*/
	set<wstring>* Get_Ingredient() { return &m_setIngredient; }	

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE	Get_InteractType() const override { return CInteract::PLATE; }

	// IPlace을(를) 통해 상속됨
	/**
	* @brief	올려놓은 재료에 맞게 접시 이미지를 교체하고, 재료는 오브젝트 풀로 반환
	*			이미 올려놓은 재료는 올리지 못하고 false를 반환
	*/
	_bool			Set_Place(CGameObject* pItem, CGameObject* pPlace) override;

	// IPlace을(를) 통해 상속됨
	_bool			Get_CanPlace(CGameObject* pItem) override;

private:
	HRESULT			Add_Component();
	_bool			Add_Ingredient(const _tchar* pTag);
	_bool			Change_Texture(const _tchar* pComponentTag);
	const _tchar*	IngredientTypeToString(CIngredient::INGREDIENT_TYPE eType);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	set<wstring>	m_setIngredient;	
	_tchar			m_szName[256];

public:
	static		CPlate* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};