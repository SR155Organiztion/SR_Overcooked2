#include "pch.h"
#include "CUi_Connectiontpage.h"
#include "CManagement.h"

CUi_Connectiontpage::CUi_Connectiontpage():CUi(nullptr)
{
}

CUi_Connectiontpage::CUi_Connectiontpage(LPDIRECT3DDEVICE9 _pGraphicDev):CUi(_pGraphicDev)
{
}

CUi_Connectiontpage::CUi_Connectiontpage(const CUi_Connectiontpage& _rhs) :CUi(_rhs)
{
}

CUi_Connectiontpage::~CUi_Connectiontpage()
{
}



HRESULT CUi_Connectiontpage::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	m_pGraphicDev = _m_pGraphicDev;

	m_fFrameDelay = 30.f;

	return S_OK;
}

int CUi_Connectiontpage::Update_GameObject(const _float& _fTimeDelta)
{

	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	if (m_iFrame < m_vecFadeoutTex.size())
	{
		m_fFrameTime += _fTimeDelta * 1000.f;
		if (m_fFrameTime >= m_fFrameDelay)
		{
			m_fFrameTime -= m_fFrameDelay;
			++m_iFrame;

			if (m_iFrame == m_vecFadeoutTex.size())
			{
				m_bLoadingComplete = true;
			}
		}
	}

	return iExit;
}

void CUi_Connectiontpage::LateUpdate_GameObject()
{

}

void CUi_Connectiontpage::Render_GameObject()
{
	if (m_iFrame >= m_vecFadeoutTex.size())
		return;
	
		m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, m_vecFadeoutTex[m_iFrame]);
	


}

HRESULT CUi_Connectiontpage::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ConnectiontPage"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });



	return S_OK;
}

CUi_Connectiontpage* CUi_Connectiontpage::Make_Connectiontpage()
{
	CUi_Connectiontpage* pGameObject = new CUi_Connectiontpage(m_pGraphicDev);
	pGameObject->Add_Component();

	for (int i = 0; i < 50; i++)
	{
		TCHAR szFileName[128] = L"";
		wsprintf(szFileName, L"../Bin/Resource/Texture/UI/in_game/Connectiontpage/UI_Connectiontpage_03_%d.png", i);

		LPDIRECT3DTEXTURE9 pTex = nullptr;
		HRESULT hr = D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, &pTex);
		pGameObject->m_vecFadeoutTex.push_back(pTex);
	}
	pGameObject->m_tData.m_vPos = D3DXVECTOR3(500, 500, 0);
	pGameObject->m_tData.m_fXScale = 1.0f;
	pGameObject->m_tData.m_fYScale = 1.0f;
	CLayer* pLayer = CManagement::GetInstance()->Get_Layer(L"UI_Layer"); //레이어 불러오기
	static _int iConnectiontpageCount = 0;
	TCHAR		szFileName[128] = L"";

	wsprintf(szFileName, L"Object_Connectiontpage%d", iConnectiontpageCount++); // 레이어 추가 및 이름 변경

	size_t len = wcslen(szFileName) + 1;
	wchar_t* pKey = new wchar_t[len];
	wcscpy_s(pKey, len, szFileName);

	if (FAILED(pLayer->Add_GameObject(pKey, pGameObject)))
		return nullptr;

	m_listData.push_back(pGameObject->m_tData);
	return pGameObject;
}

void CUi_Connectiontpage::Free()
{
}