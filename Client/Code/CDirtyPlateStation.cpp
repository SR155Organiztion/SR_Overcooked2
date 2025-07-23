#include "pch.h"
#include "CDirtyPlateStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CPlate.h"
#include "CObjectPoolMgr.h"
#include "CManagement.h"

#include "CFontMgr.h"

CDirtyPlateStation::CDirtyPlateStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CDirtyPlateStation::CDirtyPlateStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CDirtyPlateStation::~CDirtyPlateStation()
{
}

HRESULT CDirtyPlateStation::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale({ 1.f, 0.5f, 1.f });

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	return S_OK;
}

_int CDirtyPlateStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	Set_ReturnType();
	Return_Plate(fTimeDelta);
	Update_PlatePosition();

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CDirtyPlateStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Engine::CGameObject::Compute_ViewZ(&vPos);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CDirtyPlateStation::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

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
}

CGameObject* CDirtyPlateStation::Get_PlacedItem()
{
	if (m_vecItem.empty())
		return nullptr;
	
	CGameObject* pItem = m_vecItem.front();
	dynamic_cast<CInteract*>(pItem)->Set_Ground(false);
	m_vecItem.erase(m_vecItem.begin());

	if (m_vecItem.empty())
		m_pPlacedItem = nullptr;
	else
		m_pPlacedItem = m_vecItem.front();

	PlaySound_PickUp();

	return pItem;
}

HRESULT CDirtyPlateStation::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CCubeTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_CubeTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_DirtyPlate"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Alpha"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture_Alpha", pComponent });

	return S_OK;
}

void CDirtyPlateStation::Set_ReturnType()
{
	if (!m_bCheck)
	{
		CGameObject* pStation = CManagement::GetInstance()->Get_GameObject(L"GameObject_Layer", L"Sink_Wash");
		if (pStation)
			m_bDirty = true;

		m_bCheck = true;
	}
}

void CDirtyPlateStation::Return_Plate(const _float& fTimeDelta)
{
	if (CObjectPoolMgr::GetInstance()->Is_Empty(L"Plate_"))
		return;

	if (m_fTime >= m_fInterval)
	{
		CGameObject* pObj = CObjectPoolMgr::GetInstance()->Get_Object(L"Plate_");
		if (!pObj)
			return;

		CPlate* pPlate = dynamic_cast<CPlate*>(pObj);
		if (!pPlate)
			return;

		if (m_bDirty)
			pPlate->Set_State(CPlate::DIRTY);
		else
			pPlate->Set_State(CPlate::CLEAN);

		dynamic_cast<CInteract*>(pPlate)->Set_Ground(true);
		m_vecItem.push_back(pPlate);
		m_pPlacedItem = m_vecItem.front();
		CManagement::GetInstance()->Get_Layer(L"GameObject_Layer")->Add_GameObject(pPlate->Get_SelfId(), pPlate);

		m_fTime = 0.f;
	}
	else
	{
		m_fTime += fTimeDelta;
	}
}

void CDirtyPlateStation::Update_PlatePosition()
{
	_vec3 vPos{};
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	for (int i = 0; i < m_vecItem.size(); ++i)
	{
		CTransform* pItemTransform = dynamic_cast<CTransform*>(m_vecItem[i]->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		pItemTransform->Set_Pos(vPos.x, vPos.y + 0.5f + 0.2f * (m_vecItem.size() - i), vPos.z);
	}
}

CDirtyPlateStation* CDirtyPlateStation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDirtyPlateStation* pDirtyPlateStation = new CDirtyPlateStation(pGraphicDev);

	if (FAILED(pDirtyPlateStation->Ready_GameObject()))
	{
		Safe_Release(pDirtyPlateStation);
		MSG_BOX("Station_DirtyPlate Failed");
		return nullptr;
	}

	return pDirtyPlateStation;
}

void CDirtyPlateStation::Free()
{
	CInteract::Free();
}