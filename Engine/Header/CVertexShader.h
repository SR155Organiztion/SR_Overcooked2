#pragma once
#include "CShader.h"
class CVertexShader : public CShader
{
private:
	explicit CVertexShader();
	explicit CVertexShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVertexShader(const CVertexShader& rhs);
	virtual ~CVertexShader();

private:
	LPDIRECT3DVERTEXSHADER9 m_pVertexShader = nullptr;
	IDirect3DVertexDeclaration9* m_pDecl = nullptr;
	IDirect3DBaseTexture9* m_pTex;

	_matrix m_matWorld;
	_matrix m_matView;
	_matrix m_matProj;
	

public:
	HRESULT Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DVERTEXELEMENT9 _pDecl) override;
	_int Update_Component(const _float& fTimeDelta);
	void LateUpdate_Component();
	void Render_Shader(LPDIRECT3DDEVICE9 pGraphicDev) override;
	void Set_Values(_matrix* _pMatWorld, IDirect3DBaseTexture9* _pTex);

public:
	CComponent* Clone() override;
	static CVertexShader* Create(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DVERTEXELEMENT9 _pDecl);
	void Free();
};

