/**
* @file    CIngredientStation.h
* @date    2025-06-29
* @author  권예지
* @brief   재료 제공 오브젝트 클래스
* @details 플레이어가 재료를 내려놓거나 꺼낼 수 있는 공간을 제공하는 작업대 오브젝트.
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
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE	Get_InteractType() const override { return CInteract::STATION; }

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

	// IPlace을(를) 통해 상속됨
	_bool Get_CanPlace(CGameObject* pItem) override;
};