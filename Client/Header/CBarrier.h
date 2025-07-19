/**
* @file		CBarrier.h
* @date		2025-07-19
* @author	권예지
* @brief	인게임 벽 오브젝트 클래스
* @details	Stage 4
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

class CBarrier : public CGameObject
{
protected:
	explicit CBarrier(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBarrier(const CGameObject& rhs);
	virtual ~CBarrier();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	void		Set_Scale(const _float& fX, const _float& fY, const _float& fZ);

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static		CBarrier*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};