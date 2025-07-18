/**
* @file		CBrickWall.h
* @date		2025-07-18
* @author	권예지
* @brief	인게임 벽 오브젝트 클래스
* @details	Stage 1, 3
*			스케일 : x = 3  y = 2  z = 0.5 또는 1
*/
#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CBrickWall : public CGameObject
{
protected:
	explicit CBrickWall(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBrickWall(const CGameObject& rhs);
	virtual ~CBrickWall();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

private:
	HRESULT		Add_Component();
	HRESULT		Set_Metarial();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static	CBrickWall*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};