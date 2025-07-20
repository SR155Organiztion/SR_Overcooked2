#pragma once
#include "CEffect.h"
#include "Engine_Define.h"

namespace Engine {
	class CRcTex;
	class CTransform;
	class CTexture;
	class CCalculator;
}

class CCloudEffect : public CEffect
{
private:
	explicit CCloudEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCloudEffect(const CEffect& rhs);
	virtual ~CCloudEffect();

public:
	HRESULT		Ready_Effect() override;
	_int		Update_Effect(const _float& fTimeDelta) override;
	void		LateUpdate_Effect(const _float& fTimeDelta) override;
	void		Render_GameObject() override;


private:
	HRESULT		Add_Component();

	_float		m_fFrame;

public:
	virtual void	Play_Effect(_vec3 StartPos);


private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;


public:
	CEffect* Clone() override;
	static CEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	void	Free() override;

};

