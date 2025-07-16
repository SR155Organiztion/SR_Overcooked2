/**
* @file    CFireExtinguisher.h
* @date    2025-06-29
* @author  권예지
* @brief   소화기(Fire Extinguisher) 오브젝트 클래스
* @details 플레이어가 들고 이동하거나 사용하는 소화기 오브젝트.
*/
#pragma once
#include "CInteract.h"
#include "ICarry.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CFireExtinguisher : public CInteract
{
protected:
	explicit CFireExtinguisher(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFireExtinguisher(const CGameObject& rhs);
	virtual ~CFireExtinguisher();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE	Get_InteractType() const override { return CInteract::UNKNOWN; }

private:
	HRESULT		Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CFireExtinguisher* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};

