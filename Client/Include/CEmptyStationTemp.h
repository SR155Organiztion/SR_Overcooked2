/**
* @file    CEmptyStation.h
* @date    2025-06-29
* @author  권예지
* @brief   빈 작업대(Empty Station) 오브젝트 클래스
* @details 플레이어가 자유롭게 아이템을 내려놓을 수 있는 빈 공간 오브젝트.
*/
#pragma once
#include "CInteract.h"
#include "IPlace.h"
#include "IPhysics.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CEmptyStationTemp : public CInteract, public IPlace, public IPhysics
{
protected:
	explicit CEmptyStationTemp(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEmptyStationTemp(const CGameObject& rhs);
	virtual ~CEmptyStationTemp();

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

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CEmptyStationTemp* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};