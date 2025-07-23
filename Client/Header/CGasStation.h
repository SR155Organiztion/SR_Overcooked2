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
#include "IProcess.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CGasStation : public CInteract, public IPlace, public IProcess
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

	_bool			Get_Fire() { return m_bFire; }
	void			Set_Fire(_bool bFire) { m_bFire = bFire; }

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE	Get_InteractType() const override { return CInteract::STATION; }

	// IPlace을(를) 통해 상속됨
	_bool			Set_Place(CGameObject* pItem, CGameObject* pPlace) override;
	_bool			Set_Place(CGameObject* pItem) override;
	CGameObject*	Get_PlacedItem() override;
	_bool			Get_CanPlace(CGameObject* pItem) override;

	// IProcess을(를) 통해 상속됨
	_bool			Enter_Process();
	void			Update_Process(const _float& fTimeDelta);
	void			Exit_Process();

	// 
	virtual			_bool		On_Snap(CGameObject* _pGameObject) override;

private:
	HRESULT			Add_Component();

	void			Enter_Fire();

	void			PlayEffect_Loop(const _float& fTimeDelta); 
	void			PlaySound_Loop(const _float& fTimeDelta);
	void			Draw_Progress();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;;
	vector<Engine::CTexture*> m_vecTextureCom;

	_bool			m_bFire = false;

	const _float	m_fEffectIntervalInit = 0.5f;
	_float	m_fEffectInterval = 0.f;
	_float			m_fEffectTime = 0.f;

	const _float	m_fSoundIntervalInit = 10.f;
	_float			m_fSoundInterval = 0.0f;
	_float			m_fSoundTime = 0.f;

	CGameObject* m_pProgressBack = nullptr;
	CGameObject* m_pProgressFill = nullptr;
	_bool			m_bProgressVisible = false;

public:
	static CGasStation*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free(); 
};