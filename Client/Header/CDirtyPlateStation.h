/**
* @file		CDirtyPlateStation.h
* @date		2025-07-04
* @author	권예지
* @brief	더러운 접시 제공 (요리 내놓고 나면 더러운 접시 돌아옴)
* @details	
 */
#pragma once
#include "CInteract.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CDirtyPlateStation : public CInteract
{
protected:
	explicit CDirtyPlateStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDirtyPlateStation(const CGameObject& rhs);
	virtual ~CDirtyPlateStation();

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
	static CDirtyPlateStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};