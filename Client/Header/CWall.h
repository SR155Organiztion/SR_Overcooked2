/**
* @file    CWall.h
* @date    2025-06-29
* @author  권예지
* @brief   게임 내 벽 오브젝트 클래스
* @details 충돌 불가 공간 또는 배경용 벽체를 정의하는 오브젝트.
*/
#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CWall : public CGameObject
{
protected:
	explicit CWall(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWall(const CGameObject& rhs);
	virtual ~CWall();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CWall* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};

