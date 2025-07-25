#include "pch.h"
#include "CUi_Score.h"

/// <summary>
/// 사용법: Set또는 Get 함수로 외부에서 정보를 받아  m_iScore 변수에 넣는다.
/// </summary>

int CUi_Score::m_iScore = 0;
CUi_Score::CUi_Score(): CUi_Gauge(nullptr), m_pFont(nullptr), m_pSprite(nullptr), m_iPrevScore(0)
{
	
}

CUi_Score::CUi_Score(LPDIRECT3DDEVICE9 _pGraphicDev): CUi_Gauge(_pGraphicDev), m_pFont(nullptr), m_pSprite(nullptr),  m_iPrevScore(0)
{
	
}

CUi_Score::CUi_Score(const CGameObject& _rhs):CUi_Gauge(_rhs), m_pFont(nullptr), m_pSprite(nullptr), m_iPrevScore(0)
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
			m_pGraphicDev,         // 그래픽 디바이스
			50,                    // 글자 높이(픽셀)
			0,                     // 글자 너비(0=자동)
			FW_BOLD,               // 굵기
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
		m_tData.m_vPos = D3DXVECTOR3(80, 710, 0);
		m_tData.m_fXScale = 0.39999f;
		m_tData.m_fYScale = 0.7f;
		m_pGraphicDev = _m_pGraphicDev;
	}

	if (m_eGaugeType == IMAGE2_GAUGE)
	{
		for (int i = 0; i < 60; i++)
		{
			TCHAR szFileName[128] = L"";
			wsprintf(szFileName, L"../Bin/Resource/Texture/UI/in_game/Coin/UI_SpinningCoin_01_%d.png", i);

			LPDIRECT3DTEXTURE9 pTex = nullptr;
			HRESULT hr = D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, &pTex);
			m_vecCoinTex.push_back(pTex);
		}
		m_tData.m_vPos = D3DXVECTOR3(5, 710, 0);
		m_tData.m_fXScale = 0.6990999f;
		m_tData.m_fYScale = 0.6990999f;
		m_pGraphicDev = _m_pGraphicDev;
	}

	return S_OK;
}

int CUi_Score::Update_GameObject(const _float& _fTimeDelta)
{
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

	/*if (m_iScore > m_iPrevScore)
	{
		m_bCoinAnimating = true;
		m_fCoinAnimTime = 0.f;
		m_iPrevScore = m_iScore;
	}

	if (m_bCoinAnimating)
	{
		m_fCoinAnimTime += _fTimeDelta;
		if (m_fCoinAnimTime >= m_fFrameDelay)
		{
			m_fCoinAnimTime -= m_fFrameDelay;
			m_iCoinFrame = (m_iCoinFrame + 1) % m_vecCoinTex.size();
		}
		if (m_fCoinAnimTime > 1.0f) 
			m_bCoinAnimating = false;
	}*/

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
		swprintf(szScore, 32, L"%d\n", m_iScore);
		RECT rc;
		SetRect(&rc, 95, 100, 295, 555); //left, top, right, bottom
		HRESULT hr = m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		m_pFont->DrawTextW(m_pSprite, szScore, -1, &rc, DT_LEFT | DT_BOTTOM, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pSprite->End();

	}

	if (m_eGaugeType == IMAGE_GAUGE)
	{
		SetRect(m_tData.m_pSrcRect, 0, 0, 663, 468);
		m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, m_tData.m_pSrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Score0.png");
	}

	if (m_eGaugeType == LODING_GAUGE)
	{
		SetRect(m_tData.m_pSrcRect, 0, 0, 663, 468);
		m_pSpriteCom2->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, m_tData.m_pSrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Score1.png");
	}

	if (m_eGaugeType == IMAGE2_GAUGE)
	{
		if (m_iScore > m_iPrevScore)
		{
			SetRect(m_tData.m_pSrcRect, 0, 0, 300, 300);
			m_pSpriteCom3->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, m_vecCoinTex[m_iCoinFrame]);
			m_iPrevScore = m_iScore;
		}
		else
		{
			m_pSpriteCom3->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Coin/UI_SpinningCoin_01_0.png");
		}
	}
}

HRESULT CUi_Score::Add_Component()
{

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Score"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });

	pComponent = m_pSpriteCom2 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Score"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite2", pComponent });

	pComponent = m_pSpriteCom3 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Coin"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite3", pComponent });

	return S_OK;
}

void CUi_Score::Set_Score(int _iScore)
{
	m_iScore += _iScore;
}

int CUi_Score::Get_Score()
{
	return m_iScore;
}

void CUi_Score::Free()
{
   
}