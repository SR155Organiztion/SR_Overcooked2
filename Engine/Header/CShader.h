#pragma once
#include "CComponent.h"

class CShader : public CComponent
{
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

public:
	virtual HRESULT Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DVERTEXELEMENT9 _pDecl) PURE;
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

