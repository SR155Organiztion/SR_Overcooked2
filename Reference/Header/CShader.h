#pragma once
#include "CComponent.h"

BEGIN(Engine)
class ENGINE_DLL CShader : public CComponent
{
public:
	static const D3DVERTEXELEMENT9 CUBE_DECL[];
	static const D3DVERTEXELEMENT9 TEX_DECL[];

protected:
	explicit CShader();
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShader(const CShader& rhs);
	virtual ~CShader();

protected:
	LPD3DXBUFFER		m_pShaderBuf = nullptr;
	LPD3DXBUFFER		m_pErrorMsg = nullptr;
	LPD3DXCONSTANTTABLE m_pConstTable = nullptr;

	const TCHAR*		m_szShaderFileName = nullptr;
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;

public:
	virtual HRESULT Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, const D3DVERTEXELEMENT9 _pDecl[]) PURE;
	virtual _int Update_Component(const _float& fTimeDelta) PURE;
	virtual void LateUpdate_Component() PURE;
	virtual void	Render_Shader(LPDIRECT3DDEVICE9 pGraphicDev) PURE;

	void Set_ShaderFile(const TCHAR* _szFileName) {
		m_szShaderFileName = _szFileName;
	}

	LPD3DXCONSTANTTABLE Get_ConstTable() {
		return m_pConstTable;
	}

public:
	// CComponent을(를) 통해 상속됨
	virtual CComponent* Clone() override PURE;
	virtual void Free();
};

END