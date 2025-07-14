#include "pch.h"
#include "CFryingpan.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

#include "IState.h"
#include "CFontMgr.h" 
#include "CObjectPoolMgr.h"
#include "CManagement.h"

#include "CInteractMgr.h"

CFryingpan::CFryingpan(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CFryingpan::CFryingpan(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CFryingpan::~CFryingpan()
{
}

HRESULT CFryingpan::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(14.f, m_pTransformCom->Get_Scale().y, 4.f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bApplyKnockBack = true;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::TOOL, this);	// 삭제 예정

	return S_OK;
}

_int CFryingpan::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	Update_Process(fTimeDelta);
	Exit_Process();

	//swprintf_s(m_szTemp, L"후라이팬\n%f\n%d\n%d", m_fProgress, m_bGround, m_bFull);	// 디버깅

	return iExit;
}

void CFryingpan::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Engine::CGameObject::Compute_ViewZ(&vPos);

	Update_ContentPosition(this, Get_Item());

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CFryingpan::Render_GameObject()
{
	if (!Is_Full())
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

		//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		for (int i = 0; i < (int)m_bHighlight + 1; ++i)
		{
			if (m_vecTextureCom.size() > i && m_vecTextureCom[i])
			{
				m_vecTextureCom[i]->Set_Texture(0);
				if (FAILED(Set_Material()))
					return;
				m_pBufferCom->Render_Buffer();
			}
		}

		//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	//_vec2   vPos{ 100.f, 300.f };
	//CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szTemp, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));	// 디버깅
}

_bool CFryingpan::Enter_Process()
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient || CIngredient::BURNT == pIngredient->Get_State())
		return false;

	Set_Process(true);
	pIngredient->Set_State(CIngredient::COOKED);
	pIngredient->Set_Lock(true);

	return true;
}

void CFryingpan::Update_Process(const _float& fTimeDelta)
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient || CIngredient::BURNT == pIngredient->Get_State())
		return;

	if (Get_Process())
		Add_Progress(fTimeDelta, 0.2f);
}

void CFryingpan::Exit_Process()
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient)
		return;

	if (pIngredient->Get_State() == CIngredient::BURNT)
		return;

	if (Get_Progress() >= 2.f)
	{
		Set_Progress(2.f);
		pIngredient->ChangeState(new IBurntState());
		Set_Process(false);
		return;
	}

	if (Get_Progress() >= 1.f && pIngredient->Get_State() != CIngredient::DONE)
	{
		Set_Progress(1.f);
		pIngredient->ChangeState(new IDoneState());
	}
}

_bool CFryingpan::Set_Place(CGameObject* pItem, CGameObject* pPlace)
{
	if (IPlace::Set_Place(pItem, pPlace))
	{
		// 여기가 뭐다냐... 재료(밥이랑 파스타)를 냄비에 올릴 때 호출되는데 
		CIngredient* pIngredient = dynamic_cast<CIngredient*>(pItem);
		if (nullptr == pIngredient)
			return false;

		// 재료 상태 ICookState로 변경 / 충돌 끄고 / 냄비에서 재료 못 뺌
		if (pIngredient->Get_State() == CIngredient::CHOPPED)
			pIngredient->ChangeState(new ICookState());
		pIngredient->Set_Collision(false);
		pIngredient->Set_Lock(true);

		// 재료를 올렸는데, this가 가스레인지에 올라간 상태다? 그럼 Process_Enter() 호출
		if (m_bGround && m_bGasStation)
			Set_Process(true);

		return true;
	}

	return false;
}

_bool CFryingpan::Get_CanPlace(CGameObject* pItem)
{
	// 재료 (CHOP 상태의 토마토만)
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(pItem);
	if (nullptr == pIngredient)
		return false;

	if (CIngredient::TOMATOSOUP == pIngredient->Get_IngredientType())
		if (CIngredient::CHOPPED == pIngredient->Get_State())
			return true;

	return false;
}

void CFryingpan::Set_Empty()
{
	if (m_bFull)
	{
		CObjectPoolMgr::GetInstance()->Return_Object(m_pPlacedItem->Get_BaseId().c_str(), m_pPlacedItem);
		CManagement::GetInstance()->Delete_GameObject(L"GameObject_Layer", m_pPlacedItem->Get_SelfId(), m_pPlacedItem);
	}

	m_bFull = false;
	m_pPlacedItem = nullptr;

	if (dynamic_cast<IProcess*>(this))
		dynamic_cast<IProcess*>(this)->Set_Progress(0.f);
}

HRESULT CFryingpan::Add_Component()
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

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ToolTexture_Fryingpan"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ToolTexture_Fryingpan_Alpha"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture_Alpha", pComponent });

	return S_OK;
}

CFryingpan* CFryingpan::Create(LPDIRECT3DDEVICE9 pGraphicDev) 
{
	CFryingpan* pFryingpan = new CFryingpan(pGraphicDev);

	if (FAILED(pFryingpan->Ready_GameObject()))
	{
		Safe_Release(pFryingpan);
		MSG_BOX("Fryingpan Create Failed");
		return nullptr;
	}

	return pFryingpan;
}

void CFryingpan::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::TOOL, this);	// 삭제 예정
	CInteract::Free();
}