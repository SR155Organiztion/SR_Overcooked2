/**
* @file    CSinkStation.h
* @date    2025-06-29
* @author  권예지
* @brief   싱크대 오브젝트 클래스
* @details 플레이어가 재료를 세척하거나 아이템을 내려놓을 수 있는 공간 제공.
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

class CSinkStation : public CInteract, public IPlace
{
protected:
	explicit CSinkStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSinkStation(const CGameObject& rhs);
	virtual ~CSinkStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE	Get_InteractType() const override { return CInteract::SINKSTATION; }

	// IPlace을(를) 통해 상속됨
	_bool Get_CanPlace(CGameObject* pItem) override;

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CSinkStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free(); 
};