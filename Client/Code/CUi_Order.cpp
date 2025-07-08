#include "pch.h"
#include "CUi_Order.h"

CUi_Order::CUi_Order() : CUi(nullptr), m_pSpriteCom(nullptr), m_pSpriteCom2(nullptr), m_pSpriteCom3(nullptr)
, m_eObjectType(END_OBJECT), m_dwStartTime(0), m_dwLimitTime(0), m_dwTime(0)
, m_iseconds(0), m_iminute(0), m_pGauge(0), m_bVisible(false), m_dwHideTime(0)
, m_bAnimating(false), m_fAnimTime(0), m_fAnimDuration(0.5f)
{
}

CUi_Order::CUi_Order(LPDIRECT3DDEVICE9 pGraphicDev):CUi(pGraphicDev), m_pSpriteCom(nullptr), m_pSpriteCom2(nullptr), m_pSpriteCom3(nullptr)
, m_eObjectType(END_OBJECT), m_dwStartTime(0), m_dwLimitTime(0), m_dwTime(0)
, m_iseconds(0), m_iminute(0), m_pGauge(0), m_bVisible(false), m_dwHideTime(0)
{

}

CUi_Order::CUi_Order(const CUi_Order& rhs): CUi(rhs), m_pSpriteCom(nullptr), m_pSpriteCom2(nullptr), m_pSpriteCom3(nullptr)
, m_eObjectType(END_OBJECT), m_dwStartTime(0), m_dwLimitTime(0), m_dwTime(0)
, m_iseconds(0), m_iminute(0), m_pGauge(0), m_bVisible(false), m_dwHideTime(0)
{

}

CUi_Order::~CUi_Order()
{
}

HRESULT CUi_Order::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, OBJECT_TYPE _type)
{
	Get_Order(L"apple", 10.f);

	m_dwStartTime = GetTickCount64(); //시간 받아와서 수정
	m_dwLimitTime = 10000; 

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_vPos = D3DXVECTOR3(30, 20, 0);
	m_tXScale = 0.25f;
	m_tYScale = 0.35f;

	return S_OK;
}
int CUi_Order::Update_GameObject(const _float& _fTimeDelta)
{

	if (m_bAnimating)
	{
		m_fAnimTime += _fTimeDelta;
		float t = m_fAnimTime / m_fAnimDuration;
		if (t > 1.0f) 
			t = 1.0f;

		m_vPos = m_vStartPos + (m_vTargetPos - m_vStartPos) * t;

		if (t >= 1.0f)
		{
			m_bAnimating = false;
			m_vPos = m_vTargetPos; 
		}
	}

	if (m_bVisible && GetTickCount64() > m_dwHideTime)
		m_bVisible = false;

	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	return iExit;
}
void CUi_Order::LateUpdate_GameObject(const _float& _fTimeDelta)
{

}
void CUi_Order::Render_GameObject()
{
	if (!m_bVisible)
		return;

	m_dwTime = GetTickCount64();
	float remaining = (m_dwLimitTime > (m_dwTime - m_dwStartTime)) ? (m_dwLimitTime - (m_dwTime - m_dwStartTime)): 0;
	m_iminute = (int)(remaining / 1000) / 60;
	m_iseconds = (int)(remaining / 1000) % 60;

	float percent = (float)remaining / (float)m_dwLimitTime;
	if (percent < 0) 
		percent = 0;
	m_pGauge = (int)(percent * 420.0f);
	SetRect(&m_SrcRect, 0, 0, m_pGauge, 120);
	
	m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr , m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_0.png");
	m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
	m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe0.png");

	if (!m_listType.empty())
	{
		m_listType.erase(m_listType.begin());
	}
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

	//m_vecString.push_back(L"../Bin/Resource/Texture/UI/in_game/Recipe_0.png");
	///*m_vecString.push_back(L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");*/
	//m_vecString.push_back(L"../Bin/Resource/Texture/UI/in_game/Recipe0.png");
	return S_OK;
}

void CUi_Order::Get_Order(wstring _Name, float _time)
{
	m_listType.push_back(_Name);
	m_listvecTime.push_back(_time);

	m_bVisible = true;
	m_bAnimating = true;
	m_fAnimTime = 0.0f;
	m_fAnimDuration = 0.5f; 
	m_vStartPos = D3DXVECTOR3(900, 20, 0);
	m_vTargetPos = D3DXVECTOR3(30, 20, 0);
	m_vPos = m_vStartPos;
	m_dwHideTime = GetTickCount64() + static_cast<DWORD>(_time * 1000); //타이머 시작

}

void CUi_Order::Free()
{

}