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
	m_dwStartTime = GetTickCount(); //���� ����
	m_dwLimitTime = 36000; //���� �ð�

	D3DXCreateFont(
		m_pGraphicDev,         // �׷��� ����̽�
		40,                    // ���� ����(�ȼ�)
		0,                     // ���� �ʺ�(0=�ڵ�)
		FW_BOLD,               // ����
		1,                     // MipLevels
		FALSE,                 // Italic
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"����",             
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
		//Ÿ�� ����Ʈ
	}

	wchar_t szTime[32];
	swprintf(szTime, 32, L"���� �ð�: %d", m_isecondsLeft);

	RECT rc;
	SetRect(&rc, 0, 600-50, 410, 600-10); //left, top, right, bottom
	HRESULT hr = m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	if (FAILED(hr)) OutputDebugString(L"Sprite Begin ����!\n");
	m_pFont->DrawTextW(m_pSprite, szTime, -1, &rc, DT_LEFT | DT_BOTTOM,D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pSprite->End(); 


}