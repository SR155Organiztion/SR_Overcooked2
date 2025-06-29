/**
* @file    CFryingpan.h
* @date    2025-06-29
* @author  권예지
* @brief   후라이팬(Frying Pan) 오브젝트 클래스
* @details 플레이어가 들고 이동하거나, 재료를 조리하거나, 특정 재료를 올려둘 수 있는 오브젝트.
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
	// ICarry을(를) 통해 상속됨
	/**
	* @brief 해당 오브젝트가 현재 들고 이동 가능한 상태인지 확인하는 함수.
	* @return 이동 가능하면 true, 불가능하면 false.
	*/
	_bool Get_CanCarry() const override;

	// IPlace을(를) 통해 상속됨
	/**
	* @brief 해당 공간에 아이템을 내려놓을 수 있는지 확인하는 함수.
	* @param pCarry - 내려놓을 ICarry 포인터.
	* @return true면 내려놓기 가능, false면 불가능.
	*/
	_bool Get_CanPlace(ICarry* pCarry) const override;
	/**
	* @brief 해당 공간에 놓을 수 있는 아이템 타입을 설정하는 함수.
	* @details 상속 클래스에서 m_setCarryTypes에 허용할 ICarry 타입을 직접 추가.
	*/
	void Set_CarryTypes() override;

	// ICook을(를) 통해 상속됨
	/**
	* @brief 해당 재료를 가열 조리할 수 있는지 판별하는 함수.
	* @param pIngredient - 가열 조리하려는 재료 포인터.
	* @return true면 가열 조리 가능, false면 불가능.
	*/
	_bool CanCook(CIngredient* pIngredient) const override;
	/**
	* @brief 해당 재료를 조리하는 동작을 수행합니다.
	* @param pIngredient 조리 대상 재료 포인터
	* @details 실제 조리 동작을 처리하며, 상태 변경이나 진행도 증가 등이 포함될 수 있습니다.
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