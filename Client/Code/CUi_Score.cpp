#include "pch.h"
#include "CUi_Score.h"

/// <summary>
/// ����: Set�Ǵ� Get �Լ��� �ܺο��� ������ �޾�  m_iScore ������ �ִ´�.
/// </summary>

CUi_Score::CUi_Score(): CUi_Gauge(nullptr), m_pFont(nullptr), m_pSprite(nullptr), m_iScore(0), m_iPrevScore(0)
{
	
}

CUi_Score::CUi_Score(LPDIRECT3DDEVICE9 _pGraphicDev): CUi_Gauge(_pGraphicDev), m_pFont(nullptr), m_pSprite(nullptr), m_iScore(0), m_iPrevScore(0)
{
}

CUi_Score::CUi_Score(const CGameObject& _rhs):CUi_Gauge(_rhs), m_pFont(nullptr), m_pSprite(nullptr), m_iScore(0), m_iPrevScore(0)
{
}

CUi_Score::~CUi_Score()
{
}

HRESULT CUi_Score::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, GAUGE_TYPE _type)
{
	m_eGaugeType = _type;

	if (FAILED(Add_Component()))
		return E_FAIL;

	if (m_eGaugeType == FONT_GAUGE)
	{
		D3DXCreateSprite(_m_pGraphicDev, &m_pSprite);
		D3DXCreateFont(
			m_pGraphicDev,         // �׷��� ����̽�
			50,                    // ���� ����(�ȼ�)
			0,                     // ���� �ʺ�(0=�ڵ�)
			FW_BOLD,               // ����
			1,                     // MipLevels
			FALSE,                 // Italic
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			L"Naughty Squirrel",
			&m_pFont);


	}
	if (m_eGaugeType == IMAGE_GAUGE || m_eGaugeType == LODING_GAUGE)
	{
		m_vPos = D3DXVECTOR3(80, 710, 0);
		m_tXScale = 0.39999f;
		m_tYScale = 0.7f;
		m_pGraphicDev = _m_pGraphicDev;
	}

	if (m_eGaugeType == IMAGE2_GAUGE)
	{
		for (int i = 0; i < 42; i++)
		{
			TCHAR szFileName[128] = L"";
			wsprintf(szFileName, L"../Bin/Resource/Texture/UI/in_game/Coin%d.png", i);

			LPDIRECT3DTEXTURE9 pTex = nullptr;
			HRESULT hr = D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, &pTex);
			m_vecCoinTex.push_back(pTex);
		}
		m_vPos = D3DXVECTOR3(5, 710, 0);
		m_tXScale = 0.6990999f;
		m_tYScale = 0.6990999f;
		m_pGraphicDev = _m_pGraphicDev;
	}
	return S_OK;
}

int CUi_Score::Update_GameObject(const _float& _fTimeDelta)
{
	m_iScore = 1100;
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	if (m_eGaugeType == IMAGE2_GAUGE)
	{
		if (m_vecCoinTex.size() > 0)
		{
			m_fFrameTime += _fTimeDelta;
			if (m_fFrameTime >= m_fFrameDelay)
			{
				m_fFrameTime -= m_fFrameDelay;
				m_iCoinFrame = (m_iCoinFrame + 1) % m_vecCoinTex.size();
			}
		}
	}

	return iExit;
}

void CUi_Score::LateUpdate_GameObject()
{
}

void CUi_Score::Render_GameObject()
{
	
	if (m_eGaugeType == FONT_GAUGE)
	{
		wchar_t szScore[32] = { 0 };
		swprintf(szScore, 32, L"%02d\n", m_iScore);
		RECT rc;
		SetRect(&rc, 100, 100, 290, 555); //left, top, right, bottom
		HRESULT hr = m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		m_pFont->DrawTextW(m_pSprite, szScore, -1, &rc, DT_LEFT | DT_BOTTOM, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pSprite->End();

	}

	if (m_eGaugeType == IMAGE_GAUGE)
	{
		SetRect(m_pSrcRect, 0, 0, 663, 468);
		m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, m_pSrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Score0.png");
	}

	if (m_eGaugeType == LODING_GAUGE)
	{
		SetRect(m_pSrcRect, 0, 0, 663, 468);
		m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, m_pSrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Score1.png");
	}

	if (m_eGaugeType == IMAGE2_GAUGE)
	{
		m_pSpriteCom4->Render_Sprite(m_tXScale, m_tYScale, m_pSrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Coin0.png");
		if (m_iScore > m_iPrevScore)
		{
			//���� ���ۺ��� �ִϸ��̼� 
			SetRect(m_pSrcRect, 0, 0, 300, 300);
			m_pSpriteCom4->Render_Sprite(m_tXScale, m_tYScale, m_pSrcRect, m_pCenter, m_vPos, m_vecCoinTex[m_iCoinFrame]);
			m_iPrevScore = m_iScore;
		}
	}
}

void CUi_Score::Free()
{
   
}