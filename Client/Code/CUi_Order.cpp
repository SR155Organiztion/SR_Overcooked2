#include "pch.h"
#include "CUi_Order.h"

//int CUi_Order::m_iGap = 0;
//bool CUi_Order::m_bRemoved = false;
CUi_Order::CUi_Order() : CUi(nullptr), m_pSpriteCom(nullptr), m_pSpriteCom2(nullptr), m_pSpriteCom3(nullptr)
, m_iseconds(0), m_iminute(0), m_pGauge(0)
{
	
}

CUi_Order::CUi_Order(LPDIRECT3DDEVICE9 pGraphicDev):CUi(pGraphicDev), m_pSpriteCom(nullptr), m_pSpriteCom2(nullptr), m_pSpriteCom3(nullptr)
, m_iseconds(0), m_iminute(0), m_pGauge(0)
{
}

CUi_Order::CUi_Order(const CUi_Order& rhs): CUi(rhs), m_pSpriteCom(nullptr), m_pSpriteCom2(nullptr), m_pSpriteCom3(nullptr)
, m_iseconds(0), m_iminute(0), m_pGauge(0)
{

}

CUi_Order::~CUi_Order()
{
	if (m_pSpriteCom) { m_pSpriteCom->Release(); m_pSpriteCom = nullptr; }
	if (m_pSpriteCom2) { m_pSpriteCom2->Release(); m_pSpriteCom2 = nullptr; }
	if (m_pSpriteCom3) { m_pSpriteCom3->Release(); m_pSpriteCom3 = nullptr; }
}

HRESULT CUi_Order::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{
	
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}
int CUi_Order::Update_GameObject(const _float& _fTimeDelta)
{
	for (auto& data : m_listData)
	{
		if (data.m_bAnimating)
		{
			data.m_fAnimTime += _fTimeDelta;
			float t = data.m_fAnimTime / data.m_fAnimDuration;
			if (t > 1.0f)
				t = 1.0f;
			data.m_vPos = data.m_vStartPos + (data.m_vTargetPos - data.m_vStartPos) * t;

			if (t >= 1.0f)
			{
				data.m_vPos = data.m_vTargetPos;
				data.m_bAnimating = false;
			}
		}

		if (data.m_bVisible && GetTickCount64() > data.m_dwHideTime)
			data.m_bVisible = false;
	}
		_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	
	return iExit;
}

void CUi_Order::LateUpdate_GameObject(const _float& _fTimeDelta)
{
	m_tData.m_bRemove = false;
	for (auto it = m_listData.begin(); it != m_listData.end(); )
	{
		if (!it->m_bVisible || !it->m_bProcess)
		{
			if (!it->m_bVisible)
			{
				it = m_listData.erase(it);
				m_tData.m_bRemove = true;
				m_tData.m_bFail = true;
			}
			else
			{
				it = m_listData.erase(it);
				m_tData.m_bRemove = true;
			}
		}
		else
		{
			++it;
		}

	}


	if (m_tData.m_bRemove)
		OrdersAnimation();

}

void CUi_Order::Render_GameObject()
{
	for (auto& m_tData : m_listData)
	{
		if (!m_tData.m_bVisible)
			continue;
	
		m_tData.m_dwTime = GetTickCount64();
		float remaining = (m_tData.m_dwLimitTime > (m_tData.m_dwTime - m_tData.m_dwStartTime)) ? (m_tData.m_dwLimitTime - (m_tData.m_dwTime - m_tData.m_dwStartTime)) : 0;
		float percent = (float)remaining / (float)m_tData.m_dwLimitTime;
		if (percent < 0)
			percent = 0;
		m_pGauge = (int)(percent * 420.0f + 1.f);
		SetRect(&m_SrcRect, 0, 0, m_pGauge, 120);


		switch (m_tData.m_eType)
		{
		case Engine::CRecipeMgr::RECIPETYPE::SALAD_LETTUCE:
		{
			m_pSpriteCom2->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_4.png");
			m_pSpriteCom3->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, &m_SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
			m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe4.png");
		}
		break;
		case Engine::CRecipeMgr::RECIPETYPE::SALAD_LETTUCE_TOMATO:
		{
			m_pSpriteCom2->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_5.png");
			m_pSpriteCom3->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, &m_SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
			m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe5.png");
		}
		break;
		case Engine::CRecipeMgr::RECIPETYPE::SALAD_CUCUMBER_LETTUCE_TOMATO:
		{
			
			m_pSpriteCom2->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_6.png");
			m_pSpriteCom3->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, &m_SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge1.png");
			m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe6.png");
			
		}
		break;
		case Engine::CRecipeMgr::RECIPETYPE::SASHIMI_FISH:
		{
			m_pSpriteCom2->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_1.png");
			m_pSpriteCom3->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, &m_SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
			m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe1.png");
		}
		break;
		case Engine::CRecipeMgr::RECIPETYPE::SASHIMI_SHRIMP:
		{
			m_pSpriteCom2->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_0.png");
			m_pSpriteCom3->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, &m_SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
			m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe0.png");

		}
		break;
		case Engine::CRecipeMgr::RECIPETYPE::SUSHI_FISH:
		{
			m_pSpriteCom2->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_3.png");
			m_pSpriteCom3->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, &m_SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge1.png");
			m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe3.png");
		}
		break;
		case Engine::CRecipeMgr::RECIPETYPE::SUSHI_CUCUMBER:
		{
			m_pSpriteCom2->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_2.png");
			m_pSpriteCom3->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, &m_SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge1.png");
			m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe2.png");
		}
		break;
		case Engine::CRecipeMgr::RECIPETYPE::PASTA_TOMATO:
		{
			m_pSpriteCom2->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_7.png");
			m_pSpriteCom3->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, &m_SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
			m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe7.png");
		}
		break;
		}

	}
}

HRESULT CUi_Order::Add_Component()
{
    Engine::CComponent* pComponent = nullptr;

	pComponent = m_pSpriteCom2 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ObjectBox"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite2", pComponent });

	pComponent = m_pSpriteCom3 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ObjectGauge"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite3", pComponent });

	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ObjectImage"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });

	return S_OK;
}


void CUi_Order::Make_Order(Engine::CRecipeMgr::tagRecipe _Recipe)
{
	CUi_Order* pGameObject = new CUi_Order(m_pGraphicDev);
	pGameObject->Add_Component();

	pGameObject->m_tData.m_bProcess = true;
	pGameObject->m_tData.Recipe = _Recipe;
	pGameObject->m_tData.m_eType = _Recipe.eRecipeType;

	/// 보이기
	pGameObject->m_tData.m_bVisible = true;

	///크기
	pGameObject->m_tData.m_fXScale = 0.16f;
	pGameObject->m_tData.m_fYScale = 0.20f;

	///위치
	pGameObject->m_tData.m_vStartPos = D3DXVECTOR3(4500, 20, 0);
	pGameObject->m_tData.m_iGap = 490;
	
	///이동 애니메이션
	pGameObject->m_tData.m_bAnimating = true;
	pGameObject->m_tData.m_dwStartTime = GetTickCount64();
	pGameObject->m_tData.m_dwLimitTime = _Recipe.iTimeLimit * 1000;
	pGameObject->m_tData.m_fAnimTime = 0.0f;
	pGameObject->m_tData.m_fAnimDuration = 0.5f;
	pGameObject->m_tData.m_dwHideTime = 
		pGameObject->m_tData.m_dwStartTime + pGameObject->m_tData.m_dwLimitTime;

	pGameObject->m_tData.m_vPos = pGameObject->m_tData.m_vStartPos;
	pGameObject->m_tData.m_iWidth = 260;

	int xPos = 30;
	if (!m_listData.empty())
	{
		const auto& lastOrder = m_listData.back();
		xPos = (int)lastOrder.m_vTargetPos.x + lastOrder.m_iWidth * lastOrder.m_fXScale + lastOrder.m_iGap;
		pGameObject->m_tData.m_vTargetPos = D3DXVECTOR3(xPos, 20, 0);
		m_listData.push_back(pGameObject->m_tData);
	}
	else if (m_listData.empty())
	{
		pGameObject->m_tData.m_vTargetPos = D3DXVECTOR3(xPos, 20, 0);
		m_listData.push_back(pGameObject->m_tData);
	}

}

list<CUi_Order::ORDER>* CUi_Order::Get_OrderData()
{
	return &m_listData;
}


void CUi_Order::OrdersAnimation()
{
	int xPos = 30;
	
	for (auto& data : m_listData)
	{
		data.m_vStartPos = data.m_vPos;
		data.m_vTargetPos = D3DXVECTOR3((float)xPos, 20, 0);
		data.m_fAnimTime = 0.f;
		data.m_fAnimDuration = 0.5f;
		data.m_bAnimating = true;
		xPos += (int)(data.m_iWidth * data.m_fXScale) + data.m_iGap + 60;
		
	}
}


void CUi_Order::Free()
{

}