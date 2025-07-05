#include "pch.h"
#include "CUi_TimeLimit.h"
#include "CSprite.h"
#include "CProtoMgr.h"

CUi_TimeLimit::CUi_TimeLimit(): CUi_Object(nullptr),m_dwTime(0), m_dwLimitTime(0), m_dwStartTime(0), remaining(0), m_iminute(0)
{
	memset(&m_iAlpha, 255, sizeof(int[5]));
	
}

CUi_TimeLimit::CUi_TimeLimit(LPDIRECT3DDEVICE9 pGraphicDev): CUi_Object(pGraphicDev)
{
	
}

CUi_TimeLimit::CUi_TimeLimit(const CGameObject& rhs) :CUi_Object(rhs)
{
	
}

CUi_TimeLimit::~CUi_TimeLimit()
{
}

HRESULT CUi_TimeLimit::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, UI_TYPE _type)
{
	m_eType = _type;
	if (FAILED(Add_Component()))
		return E_FAIL;
	if (m_eType == TIMEFONT_OBJECT)
	{
		D3DXCreateSprite(_m_pGraphicDev, &m_pSprite);

		m_dwStartTime = GetTickCount(); //���� ����
		m_dwLimitTime = 360000; //���� �ð�

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
	if (m_eType == TIMER_OBJECT)
	{
		m_vPos = D3DXVECTOR3(1075,650, 0);
		m_tXScale = 0.499999f;
		m_tYScale = 0.75f;
	}

	if (m_eType == TIMEGAUGE_OBJECT)
	{
		m_vPos = D3DXVECTOR3(1075,650, 0);
		m_tXScale = 0.499999f;
		m_tYScale = 0.75f;
	}


	return S_OK;
}

int CUi_TimeLimit::Update_GameObject(const _float& _fTimeDelta)
{

	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	if (m_dwTime - m_dwStartTime >= m_dwLimitTime)
	{
		//Ÿ�� ����
	}
	return iExit;
}

void CUi_TimeLimit::Render_GameObject() 
{

	m_dwTime = GetTickCount();
	remaining = (m_dwLimitTime > (m_dwTime - m_dwStartTime)) ? (m_dwLimitTime - (m_dwTime - m_dwStartTime)) : 0;
	m_iminute = (remaining / 1000) / 60;
	m_iseconds = (remaining / 1000) % 60;


	if (m_eType == TIMEFONT_OBJECT)
	{
		wchar_t szTime[32];
		swprintf(szTime, 32, L"%02d:%02d\n", m_iminute, m_iseconds);
		RECT rc;
		SetRect(&rc, 800 - 220, 600 - 85, 800 - 20, 600 - 45); //left, top, right, bottom
		HRESULT hr = m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		m_pFont->DrawTextW(m_pSprite, szTime, -1, &rc, DT_LEFT | DT_BOTTOM, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pSprite->End();
	}
	
	//�̹���
	if (m_eType == TIMER_OBJECT)
	{
		m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Timer1.png");
	}
	if (m_eType == TIMEGAUGE_OBJECT)
	{
		m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Timer0.png");
	}
	
}

void CUi_TimeLimit::Free()
{

}