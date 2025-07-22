#include "pch.h"
#include "CUi_Fadeout.h"
#include "CManagement.h"

CUi_Fadeout::CUi_Fadeout():CUi()
{
}

CUi_Fadeout::CUi_Fadeout(LPDIRECT3DDEVICE9 _pGraphicDev): CUi(_pGraphicDev)
{
}

CUi_Fadeout::CUi_Fadeout(const CGameObject& _rhs): CUi(_rhs)
{
}

CUi_Fadeout::~CUi_Fadeout()
{
}

HRESULT CUi_Fadeout::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	m_pGraphicDev = _m_pGraphicDev;

	Make_Fadeout(1);

	return S_OK; 
}

int CUi_Fadeout::Update_GameObject(const _float& _fTimeDelta)
{

	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	if (m_vecFadeoutTex.size() > 0)
	{
		m_fFrameTime += _fTimeDelta * 2.5;

		if (m_fFrameTime >= m_fFrameDelay)
		{
			m_fFrameTime -= m_fFrameDelay;

			if (m_iFrame < m_vecFadeoutTex.size() - 1)
			{
				m_fFrameTime += _fTimeDelta * 2.5f;
				if (m_fFrameTime >= m_fFrameDelay)
				{
					m_fFrameTime -= m_fFrameDelay;
					++m_iFrame;

					// 프레임이 끝까지 도달했을 때 알림
					if (m_iFrame == m_vecFadeoutTex.size() - 1)
					{
						m_bFadeComplete = true; 
					}
				}
			}
		}
	}

	return iExit;
}
 
void CUi_Fadeout::LateUpdate_GameObject()
{
	
}
 
void CUi_Fadeout::Render_GameObject()
{
	if (m_vecFadeoutTex.size() > 0)
	{
		switch (m_tData.m_iNumber)
		{
		case 1:
			m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, m_vecFadeoutTex[m_iFrame]);
			break;
		case 2:
			m_pSpriteCom2->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, m_vecFadeoutTex[m_iFrame]);
			break;
		case 3:
			m_pSpriteCom3->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, m_vecFadeoutTex[m_iFrame]);
			break;
		case 4:
			m_pSpriteCom4->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, m_vecFadeoutTex[m_iFrame]);
			break;
		}
	
	}
}

HRESULT CUi_Fadeout::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Fadeout1"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });
	pComponent = m_pSpriteCom2 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Fadeout2"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite2", pComponent });
	pComponent = m_pSpriteCom3 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Fadeout3"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite3", pComponent });
	pComponent = m_pSpriteCom4 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Fadeout4"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite4", pComponent });


	return S_OK;
}

CUi_Fadeout* CUi_Fadeout::Make_Fadeout(int _number)
{
	CUi_Fadeout* pGameObject = new CUi_Fadeout(m_pGraphicDev);
	pGameObject->Add_Component();

	for (int i = 0; i <= 60; i++)
	{
		TCHAR szFileName[128] = L"";
		wsprintf(szFileName, L"../Bin/Resource/Texture/UI/in_game/Fadeout/Fadeout%d_%d.png", _number, i);

		LPDIRECT3DTEXTURE9 pTex = nullptr;
		HRESULT hr = D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, &pTex);
		m_vecFadeoutTex.push_back(pTex);
	}
	m_tData.m_vPos = D3DXVECTOR3(-50, -100, 0);
	m_tData.m_fXScale = 1.0f;
	m_tData.m_fYScale = 1.0f;
	m_tData.m_iNumber = _number;
	CLayer* pLayer = CManagement::GetInstance()->Get_Layer(L"UI_Layer"); //레이어 불러오기
	static _int iFadeoutCount = 0;
	TCHAR		szFileName[128] = L"";

	wsprintf(szFileName, L"Object_Fadeout%d_%d", _number, iFadeoutCount++); // 레이어 추가 및 이름 변경

	size_t len = wcslen(szFileName) + 1;
	wchar_t* pKey = new wchar_t[len];
	wcscpy_s(pKey, len, szFileName);

	if (FAILED(pLayer->Add_GameObject(pKey, pGameObject)))
		return nullptr;

	m_listData.push_back(pGameObject->m_tData);
	return pGameObject;
}

void CUi_Fadeout::Free()
{
}