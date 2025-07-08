#include "pch.h"
#include "CUi_Order.h"

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
}

HRESULT CUi_Order::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{
	m_iGap = 10;
	m_tXScale = 0.25f;
	m_tYScale = 0.35f;

	Get_Order(SALAD_LETTUCE, 10.f);//¡Ú½ÇÇè¿ë

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}
int CUi_Order::Update_GameObject(const _float& _fTimeDelta)
{
	for (auto& data : m_listDate)
	{
		if (data.bAnimating)
		{
			data.fAnimTime += _fTimeDelta;
			float t = data.fAnimTime / data.fAnimDuration;
			if (t > 1.0f)
				t = 1.0f;

			data.vPos = data.startPos + (data.targetPos - data.startPos) * t;

			if (t >= 1.0f)
			{
				data.vPos = data.targetPos;
				data.bAnimating = false;
			}
		}

		if (data.bVisible && GetTickCount64() > data.dwHideTime)
			data.bVisible = false;
	}
		_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	
	return iExit;
}

void CUi_Order::LateUpdate_GameObject(const _float& _fTimeDelta)
{
	for (auto it = m_listDate.begin(); it != m_listDate.end(); ) 
	{
		if (it->bVisible == false) 
		{ 
			it = m_listDate.erase(it); 
		}
		else 
		{
			++it;
		}
	}
}

void CUi_Order::Render_GameObject()
{

	for (auto& data : m_listDate)
	{
		if (!data.bVisible)
			continue;
	
		data.dwTime = GetTickCount64();
		float remaining = (data.dwLimitTime > (data.dwTime - data.dwStartTime)) ? (data.dwLimitTime - (data.dwTime - data.dwStartTime)) : 0;
		m_iminute = (int)(remaining / 1000) / 60;
		m_iseconds = (int)(remaining / 1000) % 60;

		float percent = (float)remaining / (float)data.dwLimitTime;
		if (percent < 0)
			percent = 0;
		m_pGauge = (int)(percent * 420.0f + 1.f);
		SetRect(&m_SrcRect, 0, 0, m_pGauge, 120);

		switch (data.eType)
		{
		case SALAD_LETTUCE:
		{
			m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_4.png");
			m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
			m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe4.png");
		}
		break;
		case SALAD_LETTUCE_TOMATO:
		{
			m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_5.png");
			m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
			m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe5.png");
		}
		break;
		case SALAD_CUCUMBER_LETTUCE_TOMATO:
		{
			m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_6.png");
			m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge1.png");
			m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe6.png");
			
		}
		break;
		case SASHIMI_FISH:
		{
			m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_1.png");
			m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
			m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe1.png");
		}
		break;
		case SASHIMI_SHRIMP:
		{
			m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_0.png");
			m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
			m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe0.png");

		}
		break;
		case SUSHI_FISH:
		{
			m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_3.png");
			m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge1.png");
			m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe3.png");
		}
		break;
		case SUSHI_CUCUMBER:
		{
			m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_2.png");
			m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge1.png");
			m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe2.png");
		}
		break;
		case PASTA_TOMATO:
		{
			m_pSpriteCom2->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_7.png");
			m_pSpriteCom3->Render_Sprite(m_tXScale, m_tYScale, &m_SrcRect, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge0.png");
			m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, data.vPos, L"../Bin/Resource/Texture/UI/in_game/Recipe7.png");
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
	m_tData.eType = _Name;
	m_tData.vPos = m_tData.startPos;
	m_tData.bVisible = true;
	m_tData.bAnimating = true;
	m_tData.fAnimTime = 0.0f;
	m_tData.fAnimDuration = 0.5f;
	m_tData.dwLimitTime = _time * 1000;
	m_tData.dwStartTime = GetTickCount64();
	m_tData.dwHideTime = m_tData.dwStartTime + m_tData.dwLimitTime;
	m_tData.startPos = D3DXVECTOR3(900, 20, 0); 
	switch (m_tData.eType)
	{
	case SALAD_LETTUCE:
	{
		data.width = 260;
	}
	break;
	case SALAD_LETTUCE_TOMATO:
	{
		data.width = 260;
	}
	break;
	case SALAD_CUCUMBER_LETTUCE_TOMATO:
	{
		data.width = 400;
	}
	break;
	case SASHIMI_FISH:
	{
		data.width = 260;
	}
	break;
	case SASHIMI_SHRIMP:
	{
		data.width = 260;
	}
	break;
	case SUSHI_FISH:
	{
		data.width = 400;
	}
	break;
	case SUSHI_CUCUMBER:
	{
		data.width = 400;
	}
	break;
	case PASTA_TOMATO:
	{
		data.width = 260;
	}
	break;
	}
	int xPos = 30;
	xPos += (data.width + m_iGap);
	data.bAnimating = true;
	data.targetPos = D3DXVECTOR3(xPos + (int)m_listDate.size() * (data.width + m_iGap), 20, 0);

	m_listDate.push_back(m_tData);

}

void CUi_Order::Free()
{

}