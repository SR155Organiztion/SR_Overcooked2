#include "pch.h"
#include "CFryingpan.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "IState.h"
#include "CObjectPoolMgr.h"
#include "CManagement.h"
#include "CUi_CookLoding.h"
#include "CUi_WarningBox.h"

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

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bApplyKnockBack = true;

	return S_OK;
}

_int CFryingpan::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	Update_ContentPosition(this, Get_Item());

	Update_Process(fTimeDelta);
	Exit_Process();

	Draw_Progress();
	Draw_Warning(fTimeDelta);

	_matrix matWorld;
	m_pTransformCom->Get_World(&matWorld);
	Billboard(matWorld);
	m_pTransformCom->Set_World(&matWorld);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CFryingpan::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Engine::CGameObject::Compute_ViewZ(&vPos);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CFryingpan::Render_GameObject()
{
	if (!Is_Full())
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

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

		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

_bool CFryingpan::Enter_Process()
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient || CIngredient::BURNT == pIngredient->Get_State())
		return false;

	Set_Process(true);
	pIngredient->Set_State(CIngredient::COOKED);
	pIngredient->Set_Lock(true);
	m_bProgressVisible = true;

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
		m_bWarningVisible = false;
		return;
	}

	if (Get_Progress() >= 1.f && pIngredient->Get_State() != CIngredient::DONE)
	{
		Set_Progress(1.f);
		pIngredient->ChangeState(new IDoneState());
		m_bProgressVisible = false;
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
			Enter_Process();	//Set_Process(true);

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
	{
		dynamic_cast<IProcess*>(this)->Set_Progress(0.f);
		m_bProgressVisible = false;
	}
		 
	m_fInterval = m_fIntervalInit;
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

void CFryingpan::Draw_Progress()
{
	if (m_pProgressBack && m_pProgressFill)
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
		vPos.y -= 2.f;
		vPos.z -= 1.f;

		dynamic_cast<CUi_CookLodingBox*>(m_pProgressBack)->UpdatePosition(vPos);
		dynamic_cast<CUi_CookLodingBox*>(m_pProgressBack)->On_Off(m_bProgressVisible);

		dynamic_cast<CUi_CookLoding*>(m_pProgressFill)->UpdatePosition(vPos);
		dynamic_cast<CUi_CookLoding*>(m_pProgressFill)->On_Off(m_bProgressVisible);
		dynamic_cast<CUi_CookLoding*>(m_pProgressFill)->Set_Progress(m_fProgress);
	}
	else if (!m_pProgressBack && !m_pProgressFill)
	{
		CGameObject* pProgressBack = CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_Object10");
		CGameObject* pProgressFill = CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_Object11");

		if (!pProgressBack || !pProgressFill)
			return;

		m_pProgressBack = dynamic_cast<CUi_CookLodingBox*>(pProgressBack)->Make_cookLodingBox(true);
		m_pProgressFill = dynamic_cast<CUi_CookLoding*>(pProgressFill)->Make_cookLoding(true, m_pProgressBack);
	}
}

void CFryingpan::Draw_Warning(const _float& fTimeDelta)
{
	if (m_pWarning)
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

		dynamic_cast<CUi_WarningBox*>(m_pWarning)->UpdatePosition(vPos);
		dynamic_cast<CUi_WarningBox*>(m_pWarning)->On_Off(m_bWarningVisible);

		if (!m_bGasStation)
		{
			m_bWarningVisible = false;
			return;
		}
		else
		{
			if (Get_Progress() >= 1.2f && Get_Progress() < 2.f)
			{
				m_fTime += fTimeDelta;

				if (m_fTime >= m_fInterval)
				{
					m_bWarningVisible = !m_bWarningVisible;
					m_fTime = 0.f;

					if (m_fInterval >= 0.1f)
						m_fInterval -= 0.02f;
				}
				else
				{
					m_fTime += fTimeDelta;
				}
			}
		}
	}
	else
	{
		CGameObject* pWarning = CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_Object12");

		if (!pWarning)
			return;

		m_pWarning = dynamic_cast<CUi_WarningBox*>(pWarning)->Make_WarningBox(true);
	}
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
	CInteract::Free();
}