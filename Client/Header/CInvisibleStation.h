/**
* @file    CInvisibleStation.h
* @date    2025-07-08
* @author  권예지
* @brief   투명 벽(InvisibleStation) 오브젝트 클래스
* @details 플레이어 이동 구역을 제한하기 위한 투명 벽 
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

class CInvisibleStation : public CInteract, public IPlace
{
protected:
	explicit CInvisibleStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInvisibleStation(const CGameObject& rhs);
	virtual ~CInvisibleStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE	Get_InteractType() const override { return CInteract::STATION; }

	// IPlace을(를) 통해 상속됨
	_bool Get_CanPlace(CGameObject* pItem) override;

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CInvisibleStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free(); 
};