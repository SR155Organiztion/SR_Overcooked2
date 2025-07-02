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


public:
	HRESULT	Ready_Sprite(const _tchar* pPath, SPRITE_ID eType, const _uint& iCnt, LPDIRECT3DDEVICE9 pGraphicDev);
	

	void Render_Sprite(D3DXVECTOR3 _m_vPos)
	{

		//크기 조정
		float m_fscaleX;
		float m_fscaleY;
		D3DXMATRIX m_MatScale;
		m_fscaleX = 0.15f;
		m_fscaleY = 0.15f;


		D3DXMatrixScaling(&m_MatScale, m_fscaleX, m_fscaleY, 1.0f);
		m_pSprite->SetTransform(&m_MatScale);

		m_pSprite->Begin(D3DXSPRITE_SORT_TEXTURE);
		m_pSprite->Draw(m_pTexture, nullptr, nullptr, &_m_vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pSprite->End();
	};

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