#pragma once

#include "CGameObject.h"
#include "Engine_Define.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
	class CCalculator;
}

class CEffect : public Engine::CGameObject
{
private:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Add_Component();

	_float			m_fFrame;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCalculator* m_pCalculatorCom;
	Engine::CTexture* m_pTextureCom;


public:
	static CEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};

