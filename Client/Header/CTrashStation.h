/**
* @file    CTrashStation.h
* @date    2025-06-29
* @author  권예지
* @brief   쓰레기통(Trash Station) 오브젝트 클래스
* @details 플레이어가 아이템을 버릴 수 있는 공간을 제공.
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

class CTrashStation : public CInteract, public IPlace
{
protected:
	explicit CTrashStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTrashStation(const CGameObject& rhs);
	virtual ~CTrashStation();

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
	static CTrashStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

	// IPlace을(를) 통해 상속됨
	_bool Get_CanPlace(CGameObject* pItem) override;
};