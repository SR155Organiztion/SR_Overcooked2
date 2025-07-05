#pragma once
#include "CComponent.h"
#include "CRcTex.h"

BEGIN(Engine)

class ENGINE_DLL CSprite : public CComponent
{
private:
	map< wstring, LPDIRECT3DTEXTURE9> m_mapTexture;
	LPD3DXSPRITE m_pSprite;
	LPDIRECT3DTEXTURE9 m_pTexture;


private:
	explicit CSprite();
	explicit CSprite(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSprite(const CSprite& rhs);
	~CSprite();

public:
	HRESULT	Ready_Sprite(const _tchar* pPath, const _uint& iCnt, LPDIRECT3DDEVICE9 pGraphicDev);
	void Render_SpriteAlpha(float ScaleX, float ScaleY, const RECT* m_pSrcRect, D3DXVECTOR3* m_pCenter, D3DXVECTOR3 _m_vPos, const _tchar* szKeyName, int _m_iAlpha=255);
	void Render_Sprite(float ScaleX, float ScaleY, const RECT* m_pSrcRect, D3DXVECTOR3* m_pCenter,  D3DXVECTOR3 _m_vPos, const _tchar* szKeyName);

	CSprite* Clone() override
	{
		return new CSprite(*this);
	}

	static CSprite* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, const _uint& iCnt = 1);
	
	LPD3DXSPRITE Get_Sprite() { return m_pSprite; }
	
private:
	void	Free();

};

END