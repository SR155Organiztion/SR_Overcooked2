#include "pch.h"
#include "CUi_Order.h"

CUi_Order::CUi_Order() : CUi(nullptr)
{
}

CUi_Order::CUi_Order(LPDIRECT3DDEVICE9 pGraphicDev):CUi(pGraphicDev)
{

}

CUi_Order::CUi_Order(const CUi_Order& rhs): CUi(rhs)
{

}

CUi_Order::~CUi_Order()
{
}

HRESULT CUi_Order::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, OBJECT_TYPE _type)
{
	m_dwStartTime = GetTickCount64();
	m_dwLimitTime = 10000; //���� �ð� 180000

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_vPos = D3DXVECTOR3(50, 50, 0);
	m_tXScale = 0.5f;
	m_tYScale = 0.5f;

	return S_OK;
}
int CUi_Order::Update_GameObject(const _float& _fTimeDelta)
{
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	
	return iExit;
}
void CUi_Order::LateUpdate_GameObject(const _float& _fTimeDelta)
{

}
void CUi_Order::Render_GameObject()
{
	m_dwTime = GetTickCount64();
	float remaining = (m_dwLimitTime > (m_dwTime - m_dwStartTime)) ? (m_dwLimitTime - (m_dwTime - m_dwStartTime)) : 0;
	m_iminute = (int)(remaining / 1000) / 60;
	m_iseconds = (int)(remaining / 1000) % 60;

	float percent = (float)remaining / (float)m_dwLimitTime;
	if (percent < 0) percent = 0;
	m_pGauge = (int)(percent * 420.0f);
	SetRect(m_pSrcRect, 0, 0, 440, 120);

	/*for (auto& pStr : m_vecString)
	{
		m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, m_pSrcRect, m_pCenter, m_vPos, pStr.c_str());

	}*/
	
	m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, m_pSrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_0.png");
	m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, m_pSrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
	m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, m_pSrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe0.png");
}

HRESULT CUi_Order::Add_Component()
{
    Engine::CComponent* pComponent = nullptr;


	pComponent = m_pSpriteCom2 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ObjectBox"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });

	pComponent = m_pSpriteCom3 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ObjectGauge"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });

	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ObjectImage"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });

	m_vecString.push_back(L"../Bin/Resource/Texture/UI/in_game/Recipe_0.png");
	m_vecString.push_back(L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
	m_vecString.push_back(L"../Bin/Resource/Texture/UI/in_game/Recipe0.png");

	return S_OK;
}

void CUi_Order::Free()
{

}