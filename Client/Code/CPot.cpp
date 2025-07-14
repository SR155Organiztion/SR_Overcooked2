#include "pch.h"
#include "CPot.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

#include "IState.h"
#include "CFontMgr.h"
#include "CInteractMgr.h"
#include "CObjectPoolMgr.h"
#include "CManagement.h"
#include "CUi_CookLoding.h"

CPot::CPot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CPot::CPot(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CPot::~CPot()
{
}

HRESULT CPot::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(12.f, m_pTransformCom->Get_Scale().y, 4.f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bApplyKnockBack = true;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::TOOL, this);	// 삭제 예정

	return S_OK;
}

_int CPot::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	Update_Process(fTimeDelta);
	Exit_Process();

	if (m_pProgressBack && m_pProgressFill)
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

		dynamic_cast<CUi_CookLodingBox*>(m_pProgressBack)->UpdatePosition(vPos);
		dynamic_cast<CUi_CookLoding*>(m_pProgressFill)->UpdatePosition(vPos);
		dynamic_cast<CUi_CookLoding*>(m_pProgressFill)->Set_Progress(m_fProgress);
	}
	else if (!m_pProgressBack && !m_pProgressFill)
	{
		CGameObject* pProgressBack = CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_Object10");
		CGameObject* pProgressFill = CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_Object11");

		if (!pProgressBack || !pProgressFill)
			return 0;

		m_pProgressBack = dynamic_cast<CUi_CookLodingBox*>(pProgressBack)->Make_cookLodingBox(true);
		m_pProgressFill = dynamic_cast<CUi_CookLoding*>(pProgressFill)->Make_cookLoding(true, m_pProgressBack);
	}

	_matrix matWorld;
	m_pTransformCom->Get_World(&matWorld);
	Billboard(matWorld);
	m_pTransformCom->Set_World(&matWorld);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	//swprintf_s(m_szTemp, L"냄비\n%f\n%d\n%d", m_fProgress, m_bGround, m_bFull);	// 디버깅

	return iExit;
}

void CPot::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Engine::CGameObject::Compute_ViewZ(&vPos);

	Update_ContentPosition(this, Get_Item());

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPot::Render_GameObject()
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
	//CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szTemp, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
}

_bool CPot::Enter_Process()
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient|| CIngredient::BURNT == pIngredient->Get_State())
		return false;

	Set_Process(true); 
	pIngredient->Set_State(CIngredient::COOKED);
	pIngredient->Set_Lock(true);

	return true;
}

void CPot::Update_Process(const _float& fTimeDelta)
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient || CIngredient::BURNT == pIngredient->Get_State())
		return;

	if (Get_Process())
		Add_Progress(fTimeDelta, 0.2f);		 
}

void CPot::Exit_Process()
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

_bool CPot::Set_Place(CGameObject* pItem, CGameObject* pPlace)
{
	if (IPlace::Set_Place(pItem, pPlace))
	{
		// 여기가 뭐다냐... 재료(밥이랑 파스타)를 냄비에 올릴 때 호출되는데 
		CIngredient* pIngredient = dynamic_cast<CIngredient*>(pItem);
		if (nullptr == pIngredient)
			return false;

		// 재료 상태 ICookState로 변경 / 충돌 끄고 / 냄비에서 재료 못 뺌
		if (pIngredient->Get_State() == CIngredient::RAW)
			pIngredient->ChangeState(new ICookState());
		pIngredient->Set_Collision(false);
		pIngredient->Set_Lock(true);
		
		// 재료를 올렸는데, this가 가스레인지에 올라간 상태다? 그럼 Process_Enter() 호출
		if(m_bGround && m_bGasStation)
			Set_Process(true);
		 
		return true;
	}		 

	return false;
}

_bool CPot::Get_CanPlace(CGameObject* pItem)
{
	// 재료 (RAW 상태의 쌀, 파스타만)
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(pItem);
	if (nullptr == pIngredient)
		return false;

	if (CIngredient::RICE == pIngredient->Get_IngredientType() || CIngredient::PASTA == pIngredient->Get_IngredientType())
		if (CIngredient::RAW == pIngredient->Get_State())
			return true;

	return false;
}

void CPot::Set_Empty()
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

HRESULT CPot::Add_Component()
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

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ToolTexture_Pot"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ToolTexture_Pot_Alpha"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture_Alpha", pComponent });

	return S_OK;
}

CPot* CPot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPot* pPot = new CPot(pGraphicDev);

	if (FAILED(pPot->Ready_GameObject()))
	{
		Safe_Release(pPot);
		MSG_BOX("Pot Create Failed");
		return nullptr;
	}

	return pPot;
}

void CPot::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::TOOL, this);	// 삭제 예정
	CInteract::Free();
}