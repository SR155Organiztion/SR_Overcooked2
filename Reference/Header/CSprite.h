#pragma once
#include "CComponent.h"
#include "CRcTex.h"

BEGIN(Engine)

class ENGINE_DLL CSprite : public CComponent
{
private:
	explicit CSprite();
	explicit CSprite(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSprite(const CSprite& rhs);
	~CSprite();

private:
	LPD3DXSPRITE m_pSprite;
	LPDIRECT3DTEXTURE9 m_pTexture;
	vector<LPDIRECT3DTEXTURE9> m_vecTexture; 


public:
	HRESULT	Ready_Sprite(const _tchar* pPath, SPRITE_ID eType, const _uint& iCnt, LPDIRECT3DDEVICE9 pGraphicDev);
	
	void Render_Sprite(D3DXVECTOR3 _m_vPos, int _Index = 0);
	
	CSprite* Clone() override
	{
		return new CSprite(*this);
	}

	static CSprite* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pPath,
		SPRITE_ID eType,
		const _uint& iCnt = 1);
	
	
private:
	void	Free();

};

END