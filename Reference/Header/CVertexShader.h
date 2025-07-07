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
	IDirect3DVertexDeclaration9* m_pDecl = nullptr;

	_matrix m_matWorld;
	_matrix m_matView;
	_matrix m_matProj;
	

public:
	HRESULT Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, const D3DVERTEXELEMENT9 _pDecl[]) override;
	_int Update_Component(const _float& fTimeDelta);
	void LateUpdate_Component();
	void Render_Shader(LPDIRECT3DDEVICE9 pGraphicDev) override;
	void Set_Values(_matrix* _pMatWorld);

public:
	CComponent* Clone() override;
	static CVertexShader* Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DVERTEXELEMENT9 _pDecl[]);
	void Free();
};

END