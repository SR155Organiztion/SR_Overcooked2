#include "CTexture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
{
	size_t iSize = rhs.m_vecTexture.size();

	m_vecTexture.reserve(iSize);

	m_vecTexture = rhs.m_vecTexture;

	for (size_t i = 0; i < iSize; ++i)
		m_vecTexture[i]->AddRef();
}

CTexture::~CTexture()
{
}

HRESULT CTexture::Ready_Texture(const _tchar* pPath, TEXTUREID eType, const _uint& iCnt)
{
	m_vecTexture.reserve(iCnt);

	IDirect3DBaseTexture9* pTexture = nullptr;

	for (_uint i = 0; i < iCnt; ++i)
	{
		TCHAR		szFileName[128] = L"";

		wsprintf(szFileName, pPath, i);

		HRESULT hr;
		switch (eType)
		{
			case TEX_NORMAL:

				hr = D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture);

				if (FAILED(hr))
					return E_FAIL;
				break;

			case TEX_CUBE:
				if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
					return E_FAIL;
				break;
		}

		m_vecTexture.push_back(pTexture);

	}

	return S_OK;
}

void CTexture::Set_Texture(const _uint& iIndex)
{
	if (m_vecTexture.size() <= iIndex)
		return;
	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);

}

CComponent* CTexture::Clone()
{
	return new CTexture(*this);
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTUREID eType, const _uint& iCnt)
{
	CTexture* pTexture = new CTexture(pGraphicDev);

	if (FAILED(pTexture->Ready_Texture(pPath, eType, iCnt)))
	{
		Safe_Release(pTexture);
		MSG_BOX("pTexture Create Failed");
		return nullptr;
	}

	return pTexture;
}

void CTexture::Free()
{
	for_each(m_vecTexture.begin(), m_vecTexture.end(), CDeleteObj());
	m_vecTexture.clear();
}
