#pragma once
#include "CShader.h"
class CPixelShader : public CShader
{
private:
	explicit CPixelShader();
	explicit CPixelShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPixelShader(const CPixelShader& rhs);
	virtual ~CPixelShader();

private:
	LPDIRECT3DVERTEXSHADER9 m_pVertexShader = nullptr;
	IDirect3DVertexDeclaration9* m_pDecl = nullptr;

public:
	HRESULT Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DVERTEXELEMENT9 _pDecl) override;
	_int Update_Component(const _float& fTimeDelta) override;
	void LateUpdate_Component() override;
	void Render_Shader(LPDIRECT3DDEVICE9 pGraphicDev) override;

public:
	CComponent* Clone() override;
	static CPixelShader* Create(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DVERTEXELEMENT9 _pDecl);
	void Free();
};

