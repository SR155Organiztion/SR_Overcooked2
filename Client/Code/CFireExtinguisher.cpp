#include "pch.h"
#include "CFireExtinguisher.h"
#include "CProtoMgr.h"
#include "CRenderer.h" 
#include "CInteractMgr.h"
#include "CGasStation.h"
#include "CEffectMgr.h"
#include "CFontMgr.h"
#include "CManagement.h"

CFireExtinguisher::CFireExtinguisher(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CFireExtinguisher::CFireExtinguisher(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CFireExtinguisher::~CFireExtinguisher()
{
}

HRESULT CFireExtinguisher::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bApplyKnockBack = true;

	return S_OK;
}

_int CFireExtinguisher::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	// 플레이어에서 Enter_Process, Pause_Process 호출

	//CGameObject* pPlayer = CManagement::GetInstance()->Get_GameObject(L"GameObject_Layer", L"Player2");
	//if (pPlayer)
	//{
	//	CTransform* pTransform = dynamic_cast<CTransform*>(pPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	//	pTransform->Get_Info(INFO_LOOK, &m_vLook);
	//}
	//
	//Pause_Process();
	//if (GetAsyncKeyState('2'))
	//	Enter_Process();

	Set_GasStationList();
	Update_Extinguish();

	_matrix matWorld;
	m_pTransformCom->Get_World(&matWorld);
	Billboard(matWorld);
	m_pTransformCom->Set_World(&matWorld);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	swprintf_s(m_szTemp, L"%d", m_bProcess);

	return iExit;
}

void CFireExtinguisher::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Engine::CGameObject::Compute_ViewZ(&vPos);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CFireExtinguisher::Render_GameObject()
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

	_vec2   vPos{ 100.f, 100.f };
	CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szTemp, &vPos, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
}

_bool CFireExtinguisher::Enter_Process()
{
	Set_Process(true);

	return true;
}

_bool CFireExtinguisher::Enter_Process(const _vec3& vDir)
{
	m_vLook = vDir;
	Set_Process(true);
	Engine::CEffectMgr::GetInstance()->Play_Effect(L"ExtinguishEffect", this);
	return true;
}

void CFireExtinguisher::Update_Process(const _float& fTimeDelta)
{
}

void CFireExtinguisher::Exit_Process()
{
}

HRESULT CFireExtinguisher::Add_Component()
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

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ToolTexture_FireExtinguisher"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ToolTexture_FireExtinguisher_Alpha"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture_Alpha", pComponent });

	return S_OK;
}

void CFireExtinguisher::Set_GasStationList()
{
	if (m_listGasStation.empty())
		m_listGasStation = *CInteractMgr::GetInstance()->Get_List(CInteractMgr::GASSTATION);
}

void CFireExtinguisher::Update_Extinguish()
{
	if (m_listGasStation.empty())
		return;

	for (auto pObj : m_listGasStation)
	{
		if (CGasStation* pGasStation = dynamic_cast<CGasStation*>(pObj))
		{
			if (pGasStation->Get_Fire())
			{
				_vec3 vExtinguishPos{}, vGasPos{};

				m_pTransformCom->Get_Info(INFO_POS, &vExtinguishPos);

				CTransform* pTransform = dynamic_cast<CTransform*>(pGasStation->Get_Component(ID_DYNAMIC, L"Com_Transform"));
				pTransform->Get_Info(INFO_POS, &vGasPos);

				_vec3 vTarget = vGasPos - vExtinguishPos;
				float fLength = D3DXVec3Length(&vTarget);
				D3DXVec3Normalize(&vTarget, &vTarget);

				if (m_fDistance >= fLength)
				{
					float fDot = D3DXVec3Dot(&m_vLook, &vTarget);

					if (1.f - m_fOffset <= fDot && m_bProcess)
						dynamic_cast<IProcess*>(pGasStation)->Enter_Process();
					else
						dynamic_cast<IProcess*>(pGasStation)->Pause_Process();
				}
			}
		}
	}
}

CFireExtinguisher* CFireExtinguisher::Create(LPDIRECT3DDEVICE9 pGraphicDev) 
{
	CFireExtinguisher* pFireExtinguisher = new CFireExtinguisher(pGraphicDev);

	if (FAILED(pFireExtinguisher->Ready_GameObject()))
	{
		Safe_Release(pFireExtinguisher);
		MSG_BOX("FireExtinguisher Create Failed");
		return nullptr;
	}

	return pFireExtinguisher;
}

void CFireExtinguisher::Free()
{
	CInteract::Free();
}