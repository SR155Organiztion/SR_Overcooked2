#include "pch.h"
#include "CTomatoSoup.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "IState.h"
#include "CFontMgr.h"
#include "CInteractMgr.h"

#include "IPlace.h"

CTomatoSoup::CTomatoSoup(LPDIRECT3DDEVICE9 pGraphicDev)
	: CIngredient(pGraphicDev)
{
}

CTomatoSoup::CTomatoSoup(const CGameObject& rhs)
	: CIngredient(rhs)
{
}

CTomatoSoup::~CTomatoSoup()
{
}

HRESULT CTomatoSoup::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_eIngredientType = TOMATOSOUP;
	m_eCookState = RAW;
	m_pCurrentState = new IRawState();
	m_pTransformCom->Set_Pos(10.f, m_pTransformCom->Get_Scale().y, 4.f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = true;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bApplyKnockBack = true;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::CARRY, this);	// 삭제 예정

	return S_OK;
}

_int CTomatoSoup::Update_GameObject(const _float& fTimeDelta)
{
	Draw_Icon();

	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	if (GetAsyncKeyState('T')) {	//	????
		Be_Thrown({ 1, 0, 0 }, 1);
	}

	if (m_pCurrentState)
		m_pCurrentState->Update_State(this, fTimeDelta);

	//swprintf_s(m_szTemp, L"토마토스프\n%p\n%d", m_pCurrentState, m_eCookState);	// 디버깅

	return iExit;
}

void CTomatoSoup::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTomatoSoup::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int i = 0; i < (int)m_bHighlight + 1; ++i)
	{
		if (m_vecTextureCom.size() > i && m_vecTextureCom[i])
		{
			int iIndex = 0;
			switch (m_eCookState)
			{
			case RAW: iIndex = 0; break;
			case CHOPPED: iIndex = 1; break;
			case COOKED: iIndex = 2; break;
			case DONE: iIndex = 3; break;
			case BURNT: iIndex = 4; break;
			}

			m_vecTextureCom[i]->Set_Texture(iIndex);
			if (FAILED(Set_Material()))
				return;
			m_pBufferCom->Render_Buffer();
		}
	}

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//_vec2   vPos{ 100.f, 200.f };
	//CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szTemp, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));	// 디버깅
}

HRESULT CTomatoSoup::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_IngredientTexture_Tomato"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_IngredientTexture_Tomato_Alpha"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture_Alpha", pComponent });

	return S_OK;
}

CTomatoSoup* CTomatoSoup::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTomatoSoup* pTomatoSoup = new CTomatoSoup(pGraphicDev);

	if (FAILED(pTomatoSoup->Ready_GameObject()))
	{
		Safe_Release(pTomatoSoup);
		MSG_BOX("Ingredient_TomatoSoup Create Failed");
		return nullptr;
	}

	return pTomatoSoup;
}

void CTomatoSoup::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::CARRY, this);	// 삭제 예정
	CIngredient::Free();
}