#pragma once

BEGIN(Engine)
class ENGINE_DLL Shader
{
	LPD3DXBUFFER m_pShaderCode = nullptr;
	LPD3DXBUFFER m_pErrorCode = nullptr;

	LPD3DXCONSTANTTABLE m_pConstTable = nullptr;

	const TCHAR* m_szShaderFileName = nullptr;

	virtual HRESULT Ready_Shader() = 0;

};

class Shader
{
};

END

