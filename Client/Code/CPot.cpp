#include "pch.h"
#include "CPot.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "IState.h"
#include "CObjectPoolMgr.h"
#include "CManagement.h"
#include "CUi_CookLoding.h"
#include "CUi_WarningBox.h"
#include "CUi_Icon.h"
#include "CEffectMgr.h" 

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

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bApplyKnockBack = true;

	return S_OK;
}

_int CPot::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	Update_ContentPosition(this, Get_Item());

	Update_Process(fTimeDelta);
	Exit_Process();

	Draw_Progress();
	Draw_Warning(fTimeDelta);
	Draw_Icon();
	Draw_Steam(fTimeDelta);

	PlaySound_Loop();

	_matrix matWorld;
	m_pTransformCom->Get_World(&matWorld);
	Billboard(matWorld);
	m_pTransformCom->Set_World(&matWorld);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CPot::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Engine::CGameObject::Compute_ViewZ(&vPos);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPot::Render_GameObject()
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

_bool CPot::Enter_Process()
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient|| CIngredient::BURNT == pIngredient->Get_State())
		return false;

	Set_Process(true); 
	pIngredient->Set_State(CIngredient::COOKED);
	pIngredient->Set_Lock(true);
	m_bProgressVisible = true;

	CSoundMgr::GetInstance()->Play_Sound(INGAME_HOTPOT_START, INGAME_SFX_CHANNEL);

	return true;
}

void CPot::Update_Process(const _float& fTimeDelta)
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient || CIngredient::BURNT == pIngredient->Get_State())
		return;

	if (Get_Process())
		if (Get_Progress() < 1.f)
			Add_Progress(fTimeDelta, 0.1f);
		else
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
		m_bWarningVisible = false;
		m_bSteam = false;
		return;
	}
	
	if (Get_Progress() >= 1.f && pIngredient->Get_State() != CIngredient::DONE)
	{
		Set_Progress(1.f);
		pIngredient->ChangeState(new IDoneState());
		m_bProgressVisible = false;
		m_bSteam = true;
		CSoundMgr::GetInstance()->Play_Sound(INGAME_COOKING_COOKED, INGAME_SFX_CHANNEL);
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
		if (m_bGround && m_bGasStation)
			Enter_Process();
		 
		m_bIconVisible = false;

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
	m_bIconVisible = true;
	m_bSteam = false;

	if (dynamic_cast<IProcess*>(this))
	{
		dynamic_cast<IProcess*>(this)->Set_Progress(0.f);
		m_bProgressVisible = false;
	}

	m_fInterval = m_fIntervalInit;
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

void CPot::Draw_Progress()
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

void CPot::Draw_Warning(const _float& fTimeDelta)
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

					CSoundMgr::GetInstance()->Play_Sound(INGAME_COOKING_WARNING, INGAME_SFX_CHANNEL);

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

void CPot::Draw_Icon()
{
	if (!m_pIcon)
	{
		CGameObject* pObj = CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_Object9");
		if (!pObj)
			return;

		m_pIcon = dynamic_cast<CUi_Icon*>(pObj)->Make_Icon(CIngredient::ING_END);

		CTransform* pTransform = dynamic_cast<CTransform*>(m_pIcon->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Transform"));
		_vec3 vPos{};
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
	}
	else
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		dynamic_cast<CUi_Icon*>(m_pIcon)->UpdatePosition(vPos);
		dynamic_cast<CUi_Icon*>(m_pIcon)->On_Off(m_bIconVisible);
	}
}

void CPot::Draw_Steam(const _float& fTimeDelta)
{
	if (m_bSteam)
	{
		if (m_fSteamTime >= m_fSteamInterval)
		{
			CEffectMgr::GetInstance()->Play_Effect(L"SteamEffect", this);
			m_fSteamTime = 0.f;
		}
		else
			m_fSteamTime += fTimeDelta;
	}
}

void CPot::PlaySound_Loop()
{
	if (m_bProcess && !m_bSound)
	{
		m_pSoundChannel = CSoundMgr::GetInstance()->Play_Sound(INGAME_HOTPOT_BUBBLE, INGAME_BUBBLE_CHANNEL, true, 0.f);
		m_bSound = true;
	}
	else if (!m_bProcess && m_bSound)
	{
		CSoundMgr::GetInstance()->Stop_Sound(INGAME_BUBBLE_CHANNEL, m_pSoundChannel);
		m_bSound = false;
	}
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
	CInteract::Free();
}