#include "CShader.h"

const D3DVERTEXELEMENT9 CShader::CUBE_DECL[] = {
	{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
	{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	D3DDECL_END()
};

const D3DVERTEXELEMENT9 CShader::TEX_DECL[] = {
	{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
	{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	D3DDECL_END()
};

CShader::CShader()
{
}

CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev), m_pGraphicDev(pGraphicDev)
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
	Safe_Delete(m_pShaderBuf);
	Safe_Delete(m_pErrorMsg);
	Safe_Delete(m_pConstTable);
	Safe_Delete(m_szShaderFileName);
}
