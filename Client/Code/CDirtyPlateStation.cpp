#include "pch.h"
#include "CDirtyPlateStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CPlate.h"
#include "CObjectPoolMgr.h"
#include "CManagement.h"

#include "CInteractMgr.h"
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
	m_pTransformCom->Set_Pos(9.5f, m_pTransformCom->Get_Scale().y * 0.5f, 6.5f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);	// 삭제 예정

	return S_OK;
}

_int CDirtyPlateStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	Return_Plate(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	//swprintf_s(m_szTemp, L"%f", m_fTime);	// 디버깅

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

	//_vec2   vPos{ 100.f, 100.f };
	//CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szTemp, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));	// 디버깅
}

_bool CDirtyPlateStation::Get_CanPlace(CGameObject* pItem)
{
	// 오염된 접시만
	CInteract* pInteract = dynamic_cast<CInteract*>(pItem);
	if (nullptr == pInteract)
		return false;

	CInteract::INTERACTTYPE eType = pInteract->Get_InteractType();
	if (CInteract::PLATE == eType)
		if (CPlate::DIRTY == dynamic_cast<CPlate*>(pInteract)->Get_State())
			return true;

	return false;
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

void CDirtyPlateStation::Return_Plate(const _float& fTimeDelta)
{
	if (m_bFull)
		return;

	if (CObjectPoolMgr::GetInstance()->Is_Empty(L"Tools_"))
		return;

	if (m_fTime >= m_fInterval)
	{
		CGameObject* pObj = CObjectPoolMgr::GetInstance()->Get_Object(L"Tools_");
		if (!pObj)
			return;

		CPlate* pPlate = dynamic_cast<CPlate*>(pObj);
		if (!pPlate)
			return;

		pPlate->Set_Dirty();
		Set_Place(pPlate, this);
		CManagement::GetInstance()->Get_Layer(L"GameObject_Layer")->Add_GameObject(pPlate->Get_SelfId(), pPlate);

		m_fTime = 0.f;
	}
	else
	{
		m_fTime += fTimeDelta;
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
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);	// 삭제 예정
	Engine::CGameObject::Free();
}