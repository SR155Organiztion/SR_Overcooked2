#include "CSprite.h"

CSprite::CSprite():CComponent(nullptr)
{

}

CSprite::CSprite(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}

CSprite::CSprite(const CSprite& rhs)
	: CComponent(rhs)
	, m_pSprite(rhs.m_pSprite)
	, m_pTexture(rhs.m_pTexture)
	, m_mapTexture(rhs.m_mapTexture) // 복사
{
	if (m_pSprite) m_pSprite->AddRef();
	if (m_pTexture) m_pTexture->AddRef();

	// m_mapTexture의 모든 value에 AddRef
	for (auto& pair : m_mapTexture)
		if (pair.second) pair.second->AddRef();
}


CSprite::~CSprite()
{

}

HRESULT CSprite::Ready_Sprite(const _tchar* pPath, const _uint& iCnt, LPDIRECT3DDEVICE9 pGraphicDev)
{
	
	D3DXCreateSprite(pGraphicDev, &m_pSprite);
	
	//이미지
	//m_mapTexture.clear();
	for ( int i = 0; i < iCnt; ++i)
	{
		TCHAR		szFileName[128] = L"";
		wsprintf(szFileName, pPath, i);
	
		HRESULT hr = D3DXCreateTextureFromFile(pGraphicDev, szFileName, &m_pTexture);

		if (FAILED(hr)) {
			wprintf(L"SPRITE 파일 로드 실패: %s\n", szFileName);
			return E_FAIL;
		}

		m_mapTexture[szFileName] = m_pTexture;

	}

	return S_OK;
}

void CSprite::Render_SpriteAlpha(float ScaleX, float ScaleY, const RECT* m_pSrcRect, D3DXVECTOR3* m_pCenter, D3DXVECTOR3 _m_vPos, const _tchar* szKeyName, int _m_iAlpha)
{
	//크기 조정
	float fscaleX = ScaleX;
	float fscaleY = ScaleY;
	D3DXMATRIX m_MatScale;
	D3DXMatrixScaling(&m_MatScale, fscaleX, fscaleY, 1.0f);
	m_pSprite->SetTransform(&m_MatScale);

	//그리기 
	auto it = m_mapTexture.find(szKeyName);
	if (it != m_mapTexture.end()) 
	{
		LPDIRECT3DTEXTURE9 pTex = it->second;
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		m_pSprite->Draw(pTex, m_pSrcRect, m_pCenter, &_m_vPos, D3DCOLOR_ARGB(_m_iAlpha, 255, 255, 255));
		m_pSprite->End();
	}
}

void CSprite::Render_Sprite( float ScaleX, float ScaleY, const RECT* m_pSrcRect, D3DXVECTOR3* m_pCenter, D3DXVECTOR3 _m_vPos, const _tchar* szKeyName)
{
	//크기 조정
	float fScaleX = ScaleX;
	float fScaleY = ScaleY;
	D3DXMATRIX m_MatScale;
	D3DXMatrixScaling(&m_MatScale, fScaleX, fScaleY, 1.0f);
	m_pSprite->SetTransform(&m_MatScale);


	//그리기
	auto it = m_mapTexture.find(szKeyName);
	if (it != m_mapTexture.end()) 
	{
		LPDIRECT3DTEXTURE9 pTex = it->second;
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		m_pSprite->Draw(pTex, m_pSrcRect, m_pCenter, &_m_vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pSprite->End();
	}
}

CSprite* CSprite::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, const _uint& iCnt)
{
	CSprite* pSprite = new Engine::CSprite(pGraphicDev);

		if (FAILED(pSprite->Ready_Sprite(pPath, iCnt, pGraphicDev)))
		{
			Safe_Release(pSprite);
			MSG_BOX("Sprite Create Failed");
			return nullptr;
		}

	return pSprite;
}

void CSprite::Free()
{

}