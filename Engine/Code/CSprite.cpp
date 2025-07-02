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
	, m_vecTexture(rhs.m_vecTexture)
{
}

CSprite::~CSprite()
{
}

HRESULT CSprite::Ready_Sprite(const _tchar* pPath, SPRITE_ID eType, const _uint& iCnt, LPDIRECT3DDEVICE9 pGraphicDev)
{

	D3DXCreateSprite(pGraphicDev, &m_pSprite);
	m_vecTexture.clear();
	m_vecTexture.resize(iCnt);
	
	for (_uint i = 0; i < iCnt; ++i)
	{


		TCHAR		szFileName[128] = L"";
	
		wsprintf(szFileName, pPath, i);
		
		if (FAILED(D3DXCreateTextureFromFile(pGraphicDev, szFileName, &m_pTexture)))
			return E_FAIL;
		
		m_vecTexture[i] = m_pTexture;
		//m_vecTexture.push_back(m_pTexture);
	}

	return S_OK;
}

void CSprite::Render_Sprite(D3DXVECTOR3 _m_vPos, int _Index)
{
	//크기 조정
	float m_fscaleX;
	float m_fscaleY;
	D3DXMATRIX m_MatScale;
	m_fscaleX = 0.15f;
	m_fscaleY = 0.15f;



	D3DXMatrixScaling(&m_MatScale, m_fscaleX, m_fscaleY, 1.0f);
	m_pSprite->SetTransform(&m_MatScale);

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pSprite->Draw(m_vecTexture[_Index], nullptr, nullptr, &_m_vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pSprite->End();
};

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