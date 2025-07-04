#include "pch.h"
#include "CGasStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CInteractMgr.h"
#include "CIngredient.h"

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

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);

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
	if (Get_Item())
		iIndex = 1;
	m_pTextureCom->Set_Texture(iIndex);

	m_pBufferCom->Render_Buffer();
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
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);
	Engine::CGameObject::Free();
}

_bool CGasStation::Get_CanPlace(CGameObject* pItem)
{
	// »ƒ∂Û¿Ã∆“ / ≥ø∫Ò
	CInteract* pInteract = dynamic_cast<CInteract*>(pItem);

	if (nullptr == pInteract)
		return false;

	if (CInteract::FRYINGPAN == pInteract->Get_InteractType() || CInteract::POT == pInteract->Get_InteractType())
		return true;

	return false;
}