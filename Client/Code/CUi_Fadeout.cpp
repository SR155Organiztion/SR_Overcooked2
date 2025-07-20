#include "pch.h"
#include "CUi_Fadeout.h"

CUi_Fadeout::CUi_Fadeout()
{
}

CUi_Fadeout::CUi_Fadeout(LPDIRECT3DDEVICE9 _pGraphicDev)
{
}

CUi_Fadeout::CUi_Fadeout(const CGameObject& _rhs)
{
}

CUi_Fadeout::~CUi_Fadeout()
{
}

HRESULT CUi_Fadeout::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{
	m_tData.m_vPos = D3DXVECTOR3(0, 0, 0);
	if (FAILED(Add_Component()))
		return E_FAIL;

		for (int i = 0; i < 15; i++)
		{
			TCHAR szFileName[128] = L"";
			wsprintf(szFileName, L"../Bin/Resource/Texture/UI/in_game/Fadeout1_%d.png", i);

			LPDIRECT3DTEXTURE9 pTex = nullptr;
			HRESULT hr = D3DXCreateTextureFromFile(_m_pGraphicDev, szFileName, &pTex);
			m_vecFadeoutTex.push_back(pTex);
		}
		m_tData.m_vPos = D3DXVECTOR3(-210, -250, 0);  
		m_tData.m_fXScale = 2.0f;   
		m_tData.m_fYScale = 2.0f;   
		m_pGraphicDev = _m_pGraphicDev;

	return S_OK;
}

int CUi_Fadeout::Update_GameObject(const _float& _fTimeDelta)
{

	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	if (m_vecFadeoutTex.size() > 0)
	{
		m_fFrameTime += _fTimeDelta;
		if (m_fFrameTime >= m_fFrameDelay)
		{
			m_fFrameTime -= m_fFrameDelay;
			m_iFrame = (m_iFrame + 1) % m_vecFadeoutTex.size();
		}
	}

	return iExit;
}

void CUi_Fadeout::LateUpdate_GameObject()
{
}

void CUi_Fadeout::Render_GameObject()
{
	/*RECT Rc{};
	SetRect(&Rc, 0, 0, 800, 600);

	/*m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, m_vecFadeoutTex[m_iFrame]);*/
	
	m_pSpriteCom->Render_Sprite(m_tData.m_fScale, m_tData.m_fScale, &Rc, nullptr, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Fadeout0.png");
	*/
}


HRESULT CUi_Fadeout::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Fadeout1"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });

	pComponent = m_pSpriteCom2 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Fadeout"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite2", pComponent });

	pComponent = m_pSpriteCom3 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Fadeout"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite3", pComponent });

	return S_OK;
}

void CUi_Fadeout::Free()
{
}