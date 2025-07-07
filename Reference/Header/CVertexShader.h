#pragma once
#include "CShader.h"

BEGIN(Engine)
class ENGINE_DLL CVertexShader : public CShader
{
private:
	explicit CVertexShader();
	explicit CVertexShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVertexShader(const CVertexShader& rhs);
	virtual ~CVertexShader();

private:
	LPDIRECT3DVERTEXSHADER9 m_pVertexShader = nullptr;
	LPDIRECT3DPIXELSHADER9 m_pPixelShader = nullptr;
	IDirect3DVertexDeclaration9* m_pDecl = nullptr;

	_matrix m_matWorld;
	_matrix m_matView;
	_matrix m_matProj;
	_matrix m_matCameraWorld;
	

public:
	HRESULT Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, const D3DVERTEXELEMENT9 _pDecl[]) override;
	_int Update_Component(const _float& fTimeDelta);
	void LateUpdate_Component();
	void Render_Shader(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* _pMatWorld) override;
	void End_RenderShader(LPDIRECT3DDEVICE9 pGraphicDev) override;

public:
	CComponent* Clone() override;
	static CVertexShader* Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DVERTEXELEMENT9 _pDecl[]);
	void Free();
};

END