/**
* @file    CFryingpan.h
* @date    2025-06-29
* @author  권예지
* @brief   후라이팬(Frying Pan) 오브젝트 클래스
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

class CFryingpan : public CInteract, public IPlace, public ICook
{
protected:
	explicit CFryingpan(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFryingpan(const CGameObject& rhs);
	virtual ~CFryingpan();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	// CInteract을(를) 통해 상속됨
	INTERACTTYPE	Get_InteractType() const override { return CInteract::FRYINGPAN; }

	// IProcess을(를) 통해 상속됨
	_bool			Enter_Process() override;
	void			Update_Process(const _float& fTimeDelta) override;
	void			Exit_Process() override;

	// IPlace을(를) 통해 상속됨
	_bool			Set_Place(CGameObject* pItem, CGameObject* pPlace) override;
	_bool			Get_CanPlace(CGameObject* pItem) override;
	void			Set_Empty() override;

	void			Set_GasStation(_bool bGasStation) { m_bGasStation = bGasStation; }

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	_bool			m_bGasStation;

public:
	static		CFryingpan* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};