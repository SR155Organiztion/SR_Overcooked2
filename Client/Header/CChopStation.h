/**
* @file    CChopStation.h
* @date    2025-06-29
* @author  권예지
* @brief   재료 써는 작업대 오브젝트 클래스
* @details 플레이어가 재료를 올려놓고 썰 수 있는 작업대 오브젝트.
*/
#pragma once
#include "CInteract.h"
#include "IPlace.h"
#include "IProcess.h"

#include "IPhysics.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CChopStation : public CInteract, public IPlace, public IChop//, public IPhysics
{
protected:
	explicit CChopStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CChopStation(const CGameObject& rhs);
	virtual ~CChopStation();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE	Get_InteractType() const override { return CInteract::STATION; }

	// IChop을(를) 통해 상속됨
	_bool		Enter_Process() override;
	void		Update_Process(const _float& fTimeDelta) override;
	void		Exit_Process() override;

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CChopStation*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

	// IPlace을(를) 통해 상속됨
	_bool		Get_CanPlace(CGameObject* pItem) override;
};