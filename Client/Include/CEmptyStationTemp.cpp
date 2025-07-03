#include "pch.h"
#include "CEmptyStationTemp.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CEmptyStationTemp::CEmptyStationTemp(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CEmptyStationTemp::CEmptyStationTemp(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CEmptyStationTemp::~CEmptyStationTemp()
{
}

HRESULT CEmptyStationTemp::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(10.f, m_pTransformCom->Get_Scale().y, 10.f);

	return S_OK;
}

_int CEmptyStationTemp::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CEmptyStationTemp::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CEmptyStationTemp::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

HRESULT CEmptyStationTemp::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Empty"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CEmptyStationTemp* CEmptyStationTemp::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEmptyStationTemp* pEmptyStation = new CEmptyStationTemp(pGraphicDev);

	if (FAILED(pEmptyStation->Ready_GameObject()))
	{
		Safe_Release(pEmptyStation);
		MSG_BOX("Station_Emtpy Create Failed");
		return nullptr;
	}

	return pEmptyStation;
}

void CEmptyStationTemp::Free()
{
	Engine::CGameObject::Free();
}

_bool CEmptyStationTemp::Get_CanPlace(CGameObject* pObj)
{
	return _bool();
}