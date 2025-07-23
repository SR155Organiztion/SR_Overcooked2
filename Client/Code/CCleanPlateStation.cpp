#include "pch.h"
#include "CCleanPlateStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h" 
#include "CIngredient.h"
#include "CPlate.h"

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

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	return S_OK;
}

_int CCleanPlateStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	Update_PlatePosition();

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CCleanPlateStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Engine::CGameObject::Compute_ViewZ(&vPos);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CCleanPlateStation::Render_GameObject()
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

_bool CCleanPlateStation::Set_Place(CGameObject* pItem, CGameObject* pPlace)
{
	if (nullptr == pItem || nullptr == pPlace)
		return false;

	if (m_bFull || !Get_CanPlace(pItem))	// 공간마다 올릴 수 있는 물건 종류나 조건이 다를 수 있음
		return false;

	dynamic_cast<CInteract*>(pItem)->Set_Ground(true);

	m_vecItem.push_back(pItem);
	m_pPlacedItem = m_vecItem.front();

	CSoundMgr::GetInstance()->Play_Sound(INGAME_WASHEDPLATE, INGAME_SFX_CHANNEL);

	return true;
}

_bool CCleanPlateStation::Get_CanPlace(CGameObject* pItem)
{
	// 깨끗한 접시만
	CInteract* pInteract = dynamic_cast<CInteract*>(pItem);
	if (nullptr == pInteract)
		return false;

	CInteract::INTERACTTYPE eType = pInteract->Get_InteractType();
	if (CInteract::PLATE == eType)
		if (CPlate::CLEAN == dynamic_cast<CPlate*>(pInteract)->Get_State())
			return true;

	return false;
}

CGameObject* CCleanPlateStation::Get_PlacedItem()
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

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_CleanPlate"));
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

void CCleanPlateStation::Update_PlatePosition()
{
	_vec3 vPos{};
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	for (int i = 0; i < m_vecItem.size(); ++i)
	{
		CTransform* pItemTransform = dynamic_cast<CTransform*>(m_vecItem[i]->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		pItemTransform->Set_Pos(vPos.x, vPos.y + 0.5f + 0.2f * (m_vecItem.size() - i), vPos.z);
	}
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
	CInteract::Free();
}