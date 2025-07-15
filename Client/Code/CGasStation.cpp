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

	m_pTransformCom->Set_Scale({ 1.f, 0.5f, 1.f });
	m_pTransformCom->Set_Pos(3.5f, m_pTransformCom->Get_Scale().y * 0.5f, 8.f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bIsStation = true;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);	// 삭제 예정

	return S_OK;
}

_int CGasStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CGasStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Engine::CGameObject::Compute_ViewZ(&vPos);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CGasStation::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	_bool	bProcess{};
	if (IProcess* pProcess = dynamic_cast<IProcess*>(Get_Item()))
		if (pProcess->Get_Process())
			bProcess = true;

	for (int i = 0; i < (int)m_bHighlight + 1; ++i)
	{
		if (m_vecTextureCom.size() > i && m_vecTextureCom[i])
		{
			int iIndex = (i == 0 && bProcess) ? 1 : 0;
			m_vecTextureCom[i]->Set_Texture(iIndex);
			if (FAILED(Set_Material()))
				return;
			m_pBufferCom->Render_Buffer();
		}
	}
}

_bool CGasStation::Set_Place(CGameObject* pItem, CGameObject* pPlace)
{
	if (!IPlace::Set_Place(pItem, pPlace))
		return false;

	if (IProcess* pProcess = dynamic_cast<IProcess*>(pItem))
		pProcess->Enter_Process();
	
	if (CPot* pPot = dynamic_cast<CPot*>(pItem))
		pPot->Set_GasStation(true);

	if (CFryingpan* pFryingpan = dynamic_cast<CFryingpan*>(pItem))
		pFryingpan->Set_GasStation(true);

	return true;
}

_bool CGasStation::Set_Place(CGameObject* pItem)
{
	if (!IPlace::Set_Place(pItem))
		return false;

	if (IProcess* pProcess = dynamic_cast<IProcess*>(pItem))
		pProcess->Enter_Process();

	if (CPot* pPot = dynamic_cast<CPot*>(pItem))
		pPot->Set_GasStation(true);

	if (CFryingpan* pFryingpan = dynamic_cast<CFryingpan*>(pItem))
		pFryingpan->Set_GasStation(true);

	return true;
}

CGameObject* CGasStation::Get_PlacedItem()
{
	CGameObject* pItem = IPlace::Get_PlacedItem();
	if (!pItem)
		return nullptr;

	if (IProcess* pProcess = dynamic_cast<IProcess*>(pItem))
		pProcess->Pause_Process();

	if (CPot* pPot = dynamic_cast<CPot*>(pItem))
		pPot->Set_GasStation(false);

	if (CFryingpan* pFryingpan = dynamic_cast<CFryingpan*>(pItem))
		pFryingpan->Set_GasStation(false);

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

_bool CGasStation::On_Snap(CGameObject* _pGameObject)
{
	if (dynamic_cast<CIngredient*>(_pGameObject)) {
		if (m_bFull) {
			IPlace* pTool = dynamic_cast<IPlace*>(m_pPlacedItem);
			if (pTool) {
				if (pTool->Set_Place(_pGameObject, m_pPlacedItem)) {
					dynamic_cast<CIngredient*>(_pGameObject)->Set_Ground(true);
					return true;
				}
			}
			return false;
		}
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

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Gas"));
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