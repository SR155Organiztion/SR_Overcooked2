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
#include "IProcess.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CSinkStation : public CInteract, public IPlace, public IWash
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
	_bool			Set_Place(CGameObject* pItem, CGameObject* pPlace) override;
	_bool			Get_CanPlace(CGameObject* pItem) override;
	CGameObject*	Get_PlacedItem() override;
	void			Set_Empty() override;

	// IWash을(를) 통해 상속됨
	_bool			Enter_Process() override;
	void			Update_Process(const _float& fTimeDelta) override;
	void			Exit_Process() override;

private:
	HRESULT			Add_Component();
	void			Draw_Progress();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	vector<Engine::CTexture*> m_vecTextureCom;

	CGameObject*	m_pProgressBack = nullptr;
	CGameObject*	m_pProgressFill = nullptr;
	_bool			m_bProgressVisible = false;

public:
	static CSinkStation*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();  
};