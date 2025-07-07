#include "pch.h"
#include "CCleanPlateStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CInteractMgr.h"
#include "CIngredient.h"

CCleanPlateStation::CCleanPlateStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CCleanPlateStation::CCleanPlateStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CCleanPlateStation::~CCleanPlateStation()
{
}

HRESULT CCleanPlateStation::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale({ 1.f, 0.5f, 1.f });
	//m_pTransformCom->Set_Pos(5.5f, m_pTransformCom->Get_Scale().y * 0.5f, 8.f);

	m_pTransformCom->Set_Pos(10.f, m_pTransformCom->Get_Scale().y, 10.f);
	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);

	return S_OK;
}

_int CCleanPlateStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CCleanPlateStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CCleanPlateStation::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	if (FAILED(Set_Material()))
		return;

	m_pBufferCom->Render_Buffer();
}

HRESULT CCleanPlateStation::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Plate"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CCleanPlateStation* CCleanPlateStation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCleanPlateStation* pCleanPlateStation = new CCleanPlateStation(pGraphicDev);

	if (FAILED(pCleanPlateStation->Ready_GameObject()))
	{
		Safe_Release(pCleanPlateStation);
		MSG_BOX("Station_CleanPlate Failed");
		return nullptr;
	}

	return pCleanPlateStation;
}

void CCleanPlateStation::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);
	Engine::CGameObject::Free();
}

_bool CCleanPlateStation::Get_CanPlace(CGameObject* pItem)
{
	// 더러운 접시를 시스템 내부에서는 올릴 수 있도록 처리 필요
	return false;
}