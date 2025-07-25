/**
* @file    CServingStation.h
* @date    2025-06-29
* @author  권예지
* @brief   서빙 스테이션(Serving Station) 오브젝트 클래스
* @details 플레이어가 완성된 요리를 내려놓거나 서빙할 수 있는 공간을 제공.
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

class CServingStation : public CInteract, public IPlace
{
protected:
	explicit CServingStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CServingStation(const CGameObject& rhs);
	virtual ~CServingStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE	Get_InteractType() const override { return CInteract::STATION; }

	// IPlace을(를) 통해 상속됨
	/**
	* @brief	올려놓은 재료에 맞게 접시 이미지를 교체하고, 재료는 오브젝트 풀로 반환
	*			이미 올려놓은 재료는 올리지 못하고 false를 반환
	*/
	_bool			Set_Place(CGameObject* pItem, CGameObject* pPlace) override;
	_bool			Get_CanPlace(CGameObject* pItem) override;

private:
	HRESULT			Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	vector<Engine::CTexture*> m_vecTextureCom;

public:
	static CServingStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free(); 
};