#include "CShader.h"

CShader::CShader()
{
}

CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}

CShader::CShader(const CShader& rhs)
	: CComponent(rhs), m_pShaderBuf(rhs.m_pShaderBuf)
	, m_pErrorMsg(rhs.m_pErrorMsg), m_pConstTable(rhs.m_pConstTable)
	, m_szShaderFileName(rhs.m_szShaderFileName)
{
}

CShader::~CShader()
{
}

void CShader::Free() {
	Safe_Release(m_pShaderBuf);
	Safe_Release(m_pErrorMsg);
	Safe_Release(m_pConstTable);
	Safe_Release(m_szShaderFileName);
}
