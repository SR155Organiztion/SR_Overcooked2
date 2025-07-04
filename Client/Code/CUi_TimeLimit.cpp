#include "pch.h"
#include "CUi_TimeLimit.h"

CUi_TimeLimit::CUi_TimeLimit(): CUi_Object(nullptr),m_dwTime(0), m_dwLimitTime(0), m_szTime(), m_dwStartTime(0), remaining(0)
{
}

CUi_TimeLimit::CUi_TimeLimit(LPDIRECT3DDEVICE9 pGraphicDev): CUi_Object(pGraphicDev)
{
	
}

CUi_TimeLimit::CUi_TimeLimit(const CGameObject& rhs):CUi_Object(rhs)
{

}

CUi_TimeLimit::~CUi_TimeLimit()
{
}

HRESULT CUi_TimeLimit::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{
	D3DXCreateSprite(_m_pGraphicDev, &m_pSprite);
	m_dwStartTime = GetTickCount(); //게임 시작
	m_dwLimitTime = 36000; //제한 시간

	D3DXCreateFont(
		m_pGraphicDev,         // 그래픽 디바이스
		40,                    // 글자 높이(픽셀)
		0,                     // 글자 너비(0=자동)
		FW_BOLD,               // 굵기
		1,                     // MipLevels
		FALSE,                 // Italic
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"굴림",             
		&m_pFont);

	return S_OK;
}

int CUi_TimeLimit::Update_GameObject(const _float& _fTimeDelta)
{

	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return iExit;
}

void CUi_TimeLimit::Render_GameObject() 
{

	m_dwTime = GetTickCount(); 
	remaining = (m_dwLimitTime > (m_dwTime - m_dwStartTime)) ? (m_dwLimitTime - (m_dwTime - m_dwStartTime)) : 0;
	m_isecondsLeft = remaining / 1000;

	if (m_dwTime - m_dwStartTime >= m_dwLimitTime)
	{
		//타임 리미트
	}

	wchar_t szTime[32];
	swprintf(szTime, 32, L"남은 시간: %d", m_isecondsLeft);

	RECT rc;
	SetRect(&rc, 0, 600-50, 410, 600-10); //left, top, right, bottom
	HRESULT hr = m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	if (FAILED(hr)) OutputDebugString(L"Sprite Begin 실패!\n");
	m_pFont->DrawTextW(m_pSprite, szTime, -1, &rc, DT_LEFT | DT_BOTTOM,D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pSprite->End(); 


}