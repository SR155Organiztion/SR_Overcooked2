/**
* @file    CGasStation.h
* @date    2025-06-29
* @author  권예지
* @brief   가스 스테이션(Gas Station) 오브젝트 클래스
* @details 플레이어가 가스를 채우거나 관련 아이템을 내려놓을 수 있는 공간.
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

class CGasStation : public CInteract, public IPlace
{
protected:
	explicit CGasStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGasStation(const CGameObject& rhs);
	virtual ~CGasStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE				Get_InteractType() const override { return CInteract::STATION; }

	// IPlace을(를) 통해 상속됨
	_bool						Set_Place(CGameObject* pItem, CGameObject* pPlace) override;
	CGameObject*				Get_PlacedItem() override;

	// IPlace을(를) 통해 상속됨
	_bool						Get_CanPlace(CGameObject* pItem) override;

private:
	HRESULT			Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CGasStation*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free(); 
};