/**
* @file		CHydrant.h
* @date		2025-07-19
* @author	권예지
* @brief	인게임 데코 오브젝트 클래스
* @details	Stage 1, 3
*			스케일 : 1
*/
#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CHydrant : public Engine::CGameObject
{
private:
	explicit CHydrant(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHydrant(const CGameObject& rhs);
	virtual ~CHydrant();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	void		Set_Scale(const _float& fX, const _float& fY, const _float& fZ);

private:
	HRESULT		Add_Component();
	void		BillBoard();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static		CHydrant*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};