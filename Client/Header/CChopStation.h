/**
* @file    CChopStation.h
* @date    2025-06-29
* @author  권예지
* @brief   재료 써는 작업대(Chop Station) 오브젝트 클래스
* @details 플레이어가 재료를 올려놓고 썰 수 있는 작업대 오브젝트.
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

public:
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

	// IChop을(를) 통해 상속됨

	/**
	* @brief 해당 재료를 썰 수 있는지 판별하는 함수.
	* @param pIngredient - 썰고자 하는 재료 포인터.
	* @return true면 썰기 가능, false면 불가능.
	*/
	virtual _bool CanChop(CIngredient* pIngredient) const = 0;
	/**
	* @brief 해당 재료를 써는 동작을 수행하는 함수.
	* @param pIngredient - 썰고자 하는 재료 포인터.
	* @details 실제 썰기 동작을 수행하며, 상태 변경이나 진행도 증가가 포함.
	*/
	virtual void Chop(CIngredient* pIngredient) = 0;

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
};