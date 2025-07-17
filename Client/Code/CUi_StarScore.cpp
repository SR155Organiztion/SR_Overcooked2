#include "pch.h"
#include "CUi_StarScore.h"


CUi_StarScore::CUi_StarScore(LPDIRECT3DDEVICE9 pGraphicDev): CUi(pGraphicDev)
{
}

CUi_StarScore::CUi_StarScore(const CGameObject& rhs): CUi(rhs)
{
}

CUi_StarScore::~CUi_StarScore()
{
	Free();
}

HRESULT CUi_StarScore::Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	m_tData.m_fXScale = 0.8f;
	m_tData.m_fYScale = 0.6f;
	m_tData.m_vPos = D3DXVECTOR3(0, 0, 0);

	D3DXCreateSprite(m_pGraphicDev, &m_pSprite);
	D3DXCreateFont(
		m_pGraphicDev,         // 그래픽 디바이스
		20,                    // 글자 높이(픽셀)
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

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUi_StarScore::Update_GameObject(const _float& _fTimeDelta)
{
	//m_iStarScoreNumber = 2;
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	return iExit;
}

void CUi_StarScore::LateUpdate_GameObject()
{

}

void CUi_StarScore::Render_GameObject()
{

	m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Complete0.png");
	m_pSpriteCom2->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Complete_Score0.png");

	//성공한 주문
	wchar_t szScore[32] = { 0 };
	swprintf(szScore, 32, L"%d\n", m_iDeliveredScoreNumber);
	RECT rc;
	SetRect(&rc, 500, 230, 705, 315); //left, top, right, bottom
	HRESULT hr = m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pFont->DrawTextW(m_pSprite, szScore, -1, &rc, DT_LEFT | DT_BOTTOM, D3DCOLOR_ARGB(255, 50, 50, 50));
	m_pSprite->End();

	//실패한 주문
	wchar_t szScore2[32] = { 0 };
	swprintf(szScore2, 32, L"%d\n", m_iFailedScoreNumber);
	RECT rc2;
	SetRect(&rc2, 500, 230, 705, 340); //left, top, right, bottom
	HRESULT hr2 = m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pFont->DrawTextW(m_pSprite, szScore2, -1, &rc2, DT_LEFT | DT_BOTTOM, D3DCOLOR_ARGB(255, 50, 50, 50));
	m_pSprite->End();

	//합계
	wchar_t szScore3[32] = { 0 };
	swprintf(szScore3, 32, L"%d\n", m_iTotalScoreNumber);
	RECT rc3;
	SetRect(&rc3, 500, 230, 705, 383); //left, top, right, bottom
	HRESULT hr3 = m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pFont->DrawTextW(m_pSprite, szScore3, -1, &rc3, DT_LEFT | DT_BOTTOM, D3DCOLOR_ARGB(255, 0, 0, 0));
	m_pSprite->End();


	if (m_iStarScoreNumber == 1)
	{
		SetRect(m_tData.m_pSrcRect, 0, 0, 800, 600);
		m_pSpriteCom3->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, m_tData.m_pSrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Complete_Score1.png");
	}
		

	if (m_iStarScoreNumber == 2)
	{
		SetRect(m_tData.m_pSrcRect, 0, 0, 800, 600);
		m_pSpriteCom3->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, m_tData.m_pSrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Complete_Score2.png");
	}
		

	if (m_iStarScoreNumber == 3)
	{
		SetRect(m_tData.m_pSrcRect, 0, 0, 800, 600);
		m_pSpriteCom3->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, m_tData.m_pSrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Complete_Score3.png");
	}
	
}

HRESULT CUi_StarScore::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Complete"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });

	pComponent = m_pSpriteCom2 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StarScore"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite2", pComponent });

	pComponent = m_pSpriteCom3 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StarScore"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite3", pComponent });

	return S_OK;

}

void CUi_StarScore::Free()
{
}

