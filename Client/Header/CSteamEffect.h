#pragma once
#include "CEffect.h"
#include "Engine_Define.h"

namespace Engine {
	class CRcTex;
	class CTransform;
	class CTexture;
	class CCalculator;
}

class CSteamEffect : public CEffect
{
private:
	explicit CSteamEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSteamEffect(const CEffect& rhs);
	virtual ~CSteamEffect();

public:
	virtual		HRESULT		Ready_Effect() override;
	virtual		_int		Update_Effect(const _float& fTimeDelta) override;
	virtual		void		LateUpdate_Effect(const _float& fTimeDelta) override;
	virtual		void		Render_GameObject() override;

	virtual		void		Play_Effect(_vec3 StartPos);

private:
	HRESULT		Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	_float		m_fFrame;

public:
	CSteamEffect* Clone() override;
	static CSteamEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void	Free();
};