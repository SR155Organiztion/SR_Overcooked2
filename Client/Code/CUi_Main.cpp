#include "pch.h"
#include "CUi_Main.h"

CUi_Main::CUi_Main()
{
}

CUi_Main::CUi_Main(LPDIRECT3DDEVICE9 pGraphicDev)
{
}

CUi_Main::CUi_Main(const CUi_Main& rhs)
{
}

CUi_Main::~CUi_Main()
{
}

HRESULT CUi_Main::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tData.m_vPos = { 0,0,0 };
	m_tData.m_fXScale = 0.2f;
	m_tData.m_fYScale = 0.295f;

	return S_OK;
}

int CUi_Main::Update_GameObject(const _float& _fTimeDelta)
{
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return iExit;
}

void CUi_Main::LateUpdate_GameObject(const _float& _fTimeDelta)
{
}

void CUi_Main::Render_GameObject()
{

	m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/menu/Main0.png");

}

HRESULT CUi_Main::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Main"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });

	return S_OK;
}

void CUi_Main::Free()
{
}