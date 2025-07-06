#include "pch.h"
#include "CGasStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CIngredient.h"

#include "CPot.h"
#include "CFryingpan.h"

#include "CInteractMgr.h"

CGasStation::CGasStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CGasStation::CGasStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CGasStation::~CGasStation()
{
}

HRESULT CGasStation::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(3.5f, m_pTransformCom->Get_Scale().y * 0.5f, 8.f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);	// 삭제 예정

	return S_OK;
}

_int CGasStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CGasStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CGasStation::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	int iIndex(0);
	if (IProcess* pProcess = dynamic_cast<IProcess*>(Get_Item()))
		if(pProcess->Get_Process())
			iIndex = 1;
	m_pTextureCom->Set_Texture(iIndex);

	if (FAILED(Set_Material()))
		return;

	m_pBufferCom->Render_Buffer();
}

_bool CGasStation::Set_Place(CGameObject* pItem, CGameObject* pPlace)
{
	if (!IPlace::Set_Place(pItem, pPlace))
		return false;

	if (IProcess* pProcess = dynamic_cast<IProcess*>(pItem))
		pProcess->Enter_Process();
	
	return true;
}

CGameObject* CGasStation::Get_PlacedItem()
{
	CGameObject* pItem = IPlace::Get_PlacedItem();

	if (IProcess* pProcess = dynamic_cast<IProcess*>(pItem))
		pProcess->Pause_Process();

	return pItem;
}

_bool CGasStation::Get_CanPlace(CGameObject* pItem)
{
	// 후라이팬 / 냄비
	CInteract* pInteract = dynamic_cast<CInteract*>(pItem);
	if (nullptr == pInteract)
		return false;

	CInteract::INTERACTTYPE eType = pInteract->Get_InteractType();
	switch (eType)
	{
	case CInteract::FRYINGPAN:
	case CInteract::POT:
		return true;
	}

	return false;
}

HRESULT CGasStation::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Gas"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CGasStation* CGasStation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGasStation* pGasStation = new CGasStation(pGraphicDev);

	if (FAILED(pGasStation->Ready_GameObject()))
	{
		Safe_Release(pGasStation);
		MSG_BOX("Station_Gas Failed");
		return nullptr;
	}

	return pGasStation;
}

void CGasStation::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);	// 삭제 예정
	Engine::CGameObject::Free();
}