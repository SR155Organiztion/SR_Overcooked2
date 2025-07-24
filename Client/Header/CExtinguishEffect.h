#pragma once
#include "CEffect.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CExtinguishEffect : public Engine::CEffect
{
private:
	explicit CExtinguishEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CExtinguishEffect(const CEffect& rhs);
	virtual ~CExtinguishEffect();

public:
	HRESULT		Ready_Effect() override;
	_int		Update_Effect(const _float& fTimeDelta) override;
	void		LateUpdate_Effect(const _float& fTimeDelta) override;
	void		Render_GameObject() override;


private:
	HRESULT		Add_Component();

	_float		m_fFrame;

public:
	virtual void	Play_Effect(_vec3 StartPos) override;


private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	_vec3	m_vDir{};
	_float	m_fMove{}, m_fScale{};
	
public:
	CEffect* Clone() override;
	static CEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	void	Free() override;


};

