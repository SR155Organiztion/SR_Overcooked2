#include "pch.h"
#include "CUi_TimeOut.h"

CUi_TimeOut::CUi_TimeOut():CUi_Timer(nullptr)
{
}

CUi_TimeOut::CUi_TimeOut(LPDIRECT3DDEVICE9 pGraphicDev):CUi_Timer(pGraphicDev)
{
}

CUi_TimeOut::CUi_TimeOut(const CUi_TimeOut& rhs):CUi_Timer(rhs)
{
}

CUi_TimeOut::~CUi_TimeOut()
{
	Free();
}

HRESULT CUi_TimeOut::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{

	m_tData.m_fXScale = 0.4f;
	m_tData.m_fYScale = 0.4f;
	m_tData.m_vPos = D3DXVECTOR3(500, 450, 0);

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;

}
int CUi_TimeOut::Update_GameObject(const _float& _fTimeDelta)
{
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	return iExit;
}
void CUi_TimeOut::Render_GameObject()
{

	if(m_bReady)
	m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/TimeOut0.png");
	if(m_bGo)
	m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/TimeOut1.png");
	if(m_bTimeOut)
	m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/TimeOut2.png");
}

HRESULT CUi_TimeOut::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_TimeOut"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });

	return S_OK;
}


void CUi_TimeOut::Free()
{
}