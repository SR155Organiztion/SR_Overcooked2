/**
* @file    CFloor.h
* @date    2025-06-29
* @author  권예지
* @brief   게임 내 바닥(Floor) 오브젝트 클래스
* @details 캐릭터가 이동하거나 충돌할 수 있는 기능을 하지 않고 텍스쳐만 씌운 바닥 지형을 정의하는 오브젝트.
*/
#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CFloor : public CGameObject
{
protected:
	explicit CFloor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFloor(const CGameObject& rhs);
	virtual ~CFloor();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

private:
	HRESULT		Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CFloor* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};