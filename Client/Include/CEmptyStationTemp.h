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

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE	Get_InteractType() const override { return CInteract::INGREDIENT; }

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

	// IPlace을(를) 통해 상속됨
	_bool Get_CanPlace(CGameObject* pObj) override;
};