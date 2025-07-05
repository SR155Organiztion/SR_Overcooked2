/**
* @file    CPot.h
* @date    2025-06-29
* @author  권예지
* @brief   냄비(Pot) 오브젝트 클래스
* @details 플레이어가 들고 이동하거나, 재료를 조리하거나, 특정 재료를 올려둘 수 있는 오브젝트.
*/
#pragma once
#include "CInteract.h"
#include "IPlace.h"
#include "IProcess.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CPot : public CInteract, public IPlace, public ICook
{
protected:
	explicit CPot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPot(const CGameObject& rhs);
	virtual ~CPot();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	void			Set_Placed(_bool bPlaced) { m_bPlaced = bPlaced; }

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE	Get_InteractType() const override { return CInteract::POT; }

	// IProcess을(를) 통해 상속됨
	_bool			Enter_Process() override;
	void			Update_Process(const _float& fTimeDelta) override;
	void			Exit_Process() override;

	// IPlace을(를) 통해 상속됨
	_bool			Set_Place(CGameObject* pItem, CGameObject* pPlace) override;
	_bool			Get_CanPlace(CGameObject* pItem) override;

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static		CPot*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_bool		m_bPlaced = false;

private:
	virtual		void		Free();
};