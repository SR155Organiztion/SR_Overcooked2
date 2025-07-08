#include "pch.h"
#include "CUi_Order.h"

int CUi_Order::m_iGap = 0;
CUi_Order::CUi_Order() : CUi(nullptr), m_pSpriteCom(nullptr), m_pSpriteCom2(nullptr), m_pSpriteCom3(nullptr)
, m_eObjectType(OREDER_END),m_iseconds(0), m_iminute(0), m_pGauge(0)
{
	
}

CUi_Order::CUi_Order(LPDIRECT3DDEVICE9 pGraphicDev):CUi(pGraphicDev), m_pSpriteCom(nullptr), m_pSpriteCom2(nullptr), m_pSpriteCom3(nullptr)
, m_eObjectType(OREDER_END),m_iseconds(0), m_iminute(0), m_pGauge(0)
{
}

CUi_Order::CUi_Order(const CUi_Order& rhs): CUi(rhs), m_pSpriteCom(nullptr), m_pSpriteCom2(nullptr), m_pSpriteCom3(nullptr)
, m_eObjectType(OREDER_END), m_iseconds(0), m_iminute(0), m_pGauge(0)
{

}

CUi_Order::~CUi_Order()
{
	if (m_pSpriteCom) { m_pSpriteCom->Release(); m_pSpriteCom = nullptr; }
	if (m_pSpriteCom2) { m_pSpriteCom->Release(); m_pSpriteCom = nullptr; }
	if (m_pSpriteCom3) { m_pSpriteCom->Release(); m_pSpriteCom = nullptr; }
}

HRESULT CUi_Order::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{
	m_iGap = 3;
	m_tXScale = 0.25f; //0.25
	m_tYScale = 0.35f; //0.35

	
	Get_Order(SALAD_LETTUCE, 10.f);//★실험용
	Get_Order(SASHIMI_FISH, 20.f);//★실험용
	Get_Order(SALAD_LETTUCE, 30.f);//★실험용

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

	for (auto it = m_listData.begin(); it != m_listData.end(); )
	{
		if (it->m_bVisible == false)
		{ 
			it = m_listData.erase(it);
			it->m_vPos.x = 0;
			it->m_iWidth = 0;
		}
		else 
		{
			++it;
		}
	}


}

void CUi_Order::Render_GameObject()
{

	for (auto& m_tData : m_listData)
	{
		if (!m_tData.m_bVisible)
			continue;
	
		m_tData.m_dwTime = GetTickCount64();
		float remaining = (m_tData.m_dwLimitTime > (m_tData.m_dwTime - m_tData.m_dwStartTime)) ? (m_tData.m_dwLimitTime - (m_tData.m_dwTime - m_tData.m_dwStartTime)) : 0;
		m_iminute = (int)(remaining / 1000) / 60;
		m_iseconds = (int)(remaining / 1000) % 60;

		float percent = (float)remaining / (float)m_tData.m_dwLimitTime;
		if (percent < 0)
			percent = 0;

		m_pGauge = (int)(percent * 420.0f + 1.f);
		SetRect(&m_SrcRect, 0, 0, m_pGauge, 120);

		m_pCenter = (nullptr, nullptr, nullptr);

		switch (m_tData.m_eType)
		{
		case SALAD_LETTUCE:
		{
			m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_4.png");
			m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
			m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe4.png");
		}
		break;
		case SALAD_LETTUCE_TOMATO:
		{
			m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_5.png");
			m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
			m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe5.png");
		}
		break;
		case SALAD_CUCUMBER_LETTUCE_TOMATO:
		{
			m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_6.png");
			m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge1.png");
			m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe6.png");
			
		}
		break;
		case SASHIMI_FISH:
		{
			m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_1.png");
			m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
			m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe1.png");
		}
		break;
		case SASHIMI_SHRIMP:
		{
			m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_0.png");
			m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
			m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe0.png");

		}
		break;
		case SUSHI_FISH:
		{
			m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_3.png");
			m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge1.png");
			m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe3.png");
		}
		break;
		case SUSHI_CUCUMBER:
		{
			m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_2.png");
			m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge1.png");
			m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe2.png");
		}
		break;
		case PASTA_TOMATO:
		{
			m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_7.png");
			m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
			m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe7.png");
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

void CUi_Order::Get_Order(ORDER_TYPE _Name, float _time)
{
	m_tData.m_eType = _Name;
	m_tData.m_vStartPos = D3DXVECTOR3(900, 20, 0);
	m_tData.m_vPos = m_tData.m_vStartPos;
	m_tData.m_bVisible = true;
	m_tData.m_bAnimating = true;
	m_tData.m_fAnimTime = 0.0f;
	m_tData.m_fAnimDuration = 0.5f;
	m_tData.m_dwLimitTime = _time * 1000;
	m_tData.m_dwStartTime = GetTickCount64();
	m_tData.m_dwHideTime = m_tData.m_dwStartTime + m_tData.m_dwLimitTime;
	switch (m_tData.m_eType)
	{
	case SALAD_LETTUCE:
	{
		m_tData.m_iWidth = 260;
	}
	break;
	case SALAD_LETTUCE_TOMATO:
	{
		m_tData.m_iWidth = 260;
	}
	break;
	case SALAD_CUCUMBER_LETTUCE_TOMATO:
	{
		m_tData.m_iWidth = 400;
	}
	break;
	case SASHIMI_FISH:
	{
		m_tData.m_iWidth = 260;
	}
	break;
	case SASHIMI_SHRIMP:
	{
		m_tData.m_iWidth = 260;
	}
	break;
	case SUSHI_FISH:
	{
		m_tData.m_iWidth = 400;
	}
	break;
	case SUSHI_CUCUMBER:
	{
		m_tData.m_iWidth = 400;
	}
	break;
	case PASTA_TOMATO:
	{
		m_tData.m_iWidth = 260;
	}
	break;
	}


	int xPos = 30;
	if (!m_listData.empty())
	{
		const auto& lastOrder = m_listData.back();
		xPos = (int)lastOrder.m_vTargetPos.x + lastOrder.m_iWidth * m_tXScale + m_iGap;
		m_tData.m_vTargetPos = D3DXVECTOR3(xPos, 20, 0);
		m_listData.push_back(m_tData);
	}
	else if (m_listData.empty())
	{
		m_tData.m_vTargetPos = D3DXVECTOR3 (xPos, 20, 0);
		m_listData.push_back(m_tData);
	}

}

void CUi_Order::Free()
{

}