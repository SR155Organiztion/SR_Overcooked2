#include "pch.h"
#include "CDishStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CInteractMgr.h"
#include "CIngredient.h"

CDishStation::CDishStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CDishStation::CDishStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CDishStation::~CDishStation()
{
}

HRESULT CDishStation::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(8.f, m_pTransformCom->Get_Scale().y, 10.f);

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);

	return S_OK;
}

_int CDishStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CDishStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CDishStation::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

HRESULT CDishStation::Add_Component()
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

CDishStation* CDishStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) 
{
	CDishStation* pDishStation = new CDishStation(pGraphicDev);

	if (FAILED(pDishStation->Ready_GameObject()))
	{
		Safe_Release(pDishStation);
		MSG_BOX("Station_Dish Failed");
		return nullptr;
	}

	return pDishStation;
}

void CDishStation::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);
	Engine::CGameObject::Free();
}

_bool CDishStation::Get_CanPlace(CGameObject* pItem)
{
	// 더러운 접시를 시스템 내부에서는 올릴 수 있도록 처리 필요
	return false;
}