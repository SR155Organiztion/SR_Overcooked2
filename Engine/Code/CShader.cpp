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
	{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
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
	: CComponent(rhs), m_pGraphicDev(rhs.m_pGraphicDev), m_pVsShaderBuf(rhs.m_pVsShaderBuf)
	, m_pVsErrorMsg(rhs.m_pVsErrorMsg), m_pVsConstTable(rhs.m_pVsConstTable)
	, m_pPsShaderBuf(rhs.m_pPsShaderBuf)
	, m_pPsErrorMsg(rhs.m_pPsErrorMsg), m_pPsConstTable(rhs.m_pPsConstTable)
	, m_szShaderFileName(rhs.m_szShaderFileName)
{
	if (m_pVsShaderBuf)    m_pVsShaderBuf->AddRef();
	if (m_pVsErrorMsg)     m_pVsErrorMsg->AddRef();
	if (m_pVsConstTable)   m_pVsConstTable->AddRef();
	if (m_pPsShaderBuf)    m_pPsShaderBuf->AddRef();
	if (m_pPsErrorMsg)     m_pPsErrorMsg->AddRef();
	if (m_pPsConstTable)   m_pPsConstTable->AddRef();
}

CShader::~CShader()
{
}