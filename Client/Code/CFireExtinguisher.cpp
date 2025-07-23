#include "pch.h"
#include "CFireExtinguisher.h"
#include "CProtoMgr.h"
#include "CRenderer.h" 
#include "CInteractMgr.h"
#include "CGasStation.h"
#include "CEffectMgr.h"
#include "CManagement.h"

const _vec3 CFireExtinguisher::vDiagonal[12] =
{
	{  0.000f, 0.f, -1.000f },  // 90도  → 북
	{ -0.500f, 0.f, -0.866f },  // 120도 → 북서
	{ -0.866f, 0.f, -0.500f },  // 150도 → 서북서
	{ -1.000f, 0.f,  0.000f },  // 180도 → 서
	{ -0.866f, 0.f,  0.500f },  // 210도 → 서남서
	{ -0.500f, 0.f,  0.866f },  // 240도 → 남서
	{  0.000f, 0.f,  1.000f },  // 270도 → 남
	{  0.500f, 0.f,  0.866f },  // 300도 → 남동
	{  0.866f, 0.f,  0.500f },  // 330도 → 동남동
	{  1.000f, 0.f,  0.000f },  // 0도   → 동
	{  0.866f, 0.f, -0.500f },  // 30도  → 동북동
	{  0.500f, 0.f, -0.866f },  // 60도  → 북동
};

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

	Set_GasStationList();
	Update_Extinguish();

	Update_Process(fTimeDelta);

	PlaySound_Loop(fTimeDelta);

	_matrix matWorld;
	m_pTransformCom->Get_World(&matWorld);
	Billboard(matWorld);
	m_pTransformCom->Set_World(&matWorld);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

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

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	int iIndex{};
	float fMaxDot = -1.f;

	for (int i = 0; i < 12; ++i)
	{
		float fDot = D3DXVec3Dot(&m_vLook, &vDiagonal[i]);
		if (fDot > fMaxDot)
		{
			fMaxDot = fDot;
			iIndex = i;
		}
	}

	for (int i = 0; i < (int)m_bHighlight + 1; ++i)
	{
		if (m_vecTextureCom.size() > i && m_vecTextureCom[i])
		{
			m_vecTextureCom[i]->Set_Texture(iIndex);
			if (FAILED(Set_Material()))
				return;
			m_pBufferCom->Render_Buffer();
		}
	}

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
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

	return true;
}

void CFireExtinguisher::Update_Process(const _float& fTimeDelta)
{
	if (m_bProcess)
	{
		if (m_fTime >= m_fInterval)
		{
			Engine::CEffectMgr::GetInstance()->Play_Effect(L"ExtinguishEffect", this);
			m_fTime = 0.f;
		}
		else
			m_fTime += fTimeDelta;
	}
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

void CFireExtinguisher::PlaySound_Loop(const _float& fTimeDelta)
{
	if (m_bProcess)
	{
		if (m_fSoundTime >= m_fSoundInterval)
		{
			m_ExtinguisherSoundChannel 
				= CSoundMgr::GetInstance()
					->Play_Sound(
						INGAME_EXTINGUISHER_LOOP, INGAME_SFX
						, true, 0.f);

			m_fSoundTime = 0.f;
			m_fSoundInterval = m_fSoundIntervalInit;
		}
		else
			m_fSoundTime += fTimeDelta;
	}
	else
	{
		if (m_fSoundTime > 0.f) {
			int i = 0;
		}

		CSoundMgr::GetInstance()->Stop_Sound(INGAME_SFX, m_ExtinguisherSoundChannel);
		m_fSoundTime = 0.f;
		m_fSoundInterval = 0.f;
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