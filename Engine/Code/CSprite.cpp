#include "CSprite.h"

CSprite::CSprite()
{

}

CSprite::CSprite(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}

CSprite::CSprite(const CSprite& rhs) : 
	CComponent(rhs)
	, m_pSprite(rhs.m_pSprite)
	, m_pTexture(rhs.m_pTexture)
{

}

CSprite::~CSprite()
{
}

HRESULT CSprite::Ready_Sprite(const _tchar* pPath, SPRITE_ID eType, const _uint& iCnt, LPDIRECT3DDEVICE9 pGraphicDev)
{
	D3DXCreateSprite(pGraphicDev, &m_pSprite);

	for (_uint i = 0; i < iCnt; ++i)
	{
		TCHAR		szFileName[128] = L"";

		wsprintf(szFileName, pPath, i);
		

		switch (eType)
		{
		case SPRITE_BUTTON:
			
			if (FAILED(D3DXCreateTextureFromFile(pGraphicDev, szFileName, &m_pTexture)))
				return E_FAIL;
			break;
		}


	}

	return S_OK;
}


CSprite* CSprite::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, SPRITE_ID eType, const _uint& iCnt)
{
	CSprite* pSprite = new Engine::CSprite(pGraphicDev);
	


	if (FAILED(pSprite->Ready_Sprite(pPath, eType, iCnt , pGraphicDev)))
	{
		Safe_Release(pSprite);
		MSG_BOX("pTexture & Sprite Create Failed");
		return nullptr;
	}

	return pSprite;
}




void CSprite::Free()
{

}