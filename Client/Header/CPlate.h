/**
* @file    CPlate.h
* @date    2025-06-29
* @author  권예지
* @brief   접시(Plate) 오브젝트 클래스
* @details 플레이어가 들고 이동하거나, 재료를 담거나, 특정 재료를 올려둘 수 있는 오브젝트.
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
	// ICarry을(를) 통해 상속됨
	/**
	* @brief 해당 오브젝트가 현재 들고 이동 가능한 상태인지 확인하는 함수.
	* @return 이동 가능하면 true, 불가능하면 false.
	*/
	_bool Get_CanCarry() const override;

	// IPlate을(를) 통해 상속됨
	/**
	* @brief 해당 재료를 담을 수 있는지 판별하는 함수.
	* @param pIngredient - 담으려는 재료 포인터.
	* @return 담을 수 있으면 true, 불가능하면 false.
	*/
	_bool CanPlate(CIngredient* pIngredient) const override;
	/**
	* @brief 해당 재료를 담는 동작을 수행하는 함수.
	* @param pIngredient - 담을 재료 포인터.
	* @details 실제 재료를 접시에 올리는 동작을 처리.
	*/
	void Plate(CIngredient* pIngredient) override;

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE	Get_InteractType() const override { return CInteract::PLATE; }

	// IPlace을(를) 통해 상속됨
	_bool			Set_Place(CGameObject* pItem, CGameObject* pPlace) override;

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

	// IPlace을(를) 통해 상속됨
	_bool Get_CanPlace(CGameObject* pItem) override;
};