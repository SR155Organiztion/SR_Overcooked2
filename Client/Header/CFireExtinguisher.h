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

class CFireExtinguisher : public CInteract, public ICarry
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

public:
	// ICarry을(를) 통해 상속됨
	/**
	* @brief 해당 오브젝트가 현재 들고 이동 가능한 상태인지 확인하는 함수.
	* @return 이동 가능하면 true, 불가능하면 false.
	*/
	_bool Get_CanCarry() const override;

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

