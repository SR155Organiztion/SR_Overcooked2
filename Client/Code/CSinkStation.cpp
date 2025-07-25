#include "pch.h"
#include "CSinkStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CPlate.h"
#include "CManagement.h"
#include "CUi_CookLoding.h"
#include "CManagement.h"

CSinkStation::CSinkStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CSinkStation::CSinkStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CSinkStation::~CSinkStation()
{
}

HRESULT CSinkStation::Ready_GameObject()
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

_int CSinkStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	Update_Process(fTimeDelta);
	Exit_Process();

	Draw_Progress();

	return iExit;
}

void CSinkStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Engine::CGameObject::Compute_ViewZ(&vPos);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CSinkStation::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	for (int i = 0; i < (int)m_bHighlight + 1; ++i)
	{
		if (m_vecTextureCom.size() > i && m_vecTextureCom[i])
		{
			int iIndex = (i == 0 && m_bProcess) ? 1 : 0;

			m_vecTextureCom[i]->Set_Texture(iIndex);
			if (FAILED(Set_Material()))
				return;
			m_pBufferCom->Render_Buffer();
		}
	}
}

_bool CSinkStation::Set_Place(CGameObject* pItem, CGameObject* pPlace)
{
	if (nullptr == pItem || nullptr == pPlace)
		return false;

	if (m_bFull || !Get_CanPlace(pItem))	// 공간마다 올릴 수 있는 물건 종류나 조건이 다를 수 있음
		return false;

	CTransform* pItemTransform = dynamic_cast<CTransform*>(pItem->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	CTransform* pPlaceTransform = dynamic_cast<CTransform*>(pPlace->Get_Component(ID_DYNAMIC, L"Com_Transform"));

	_vec3 vPlacePos{}, vPlaceScale{}, vItemScale{};
	pPlaceTransform->Get_Info(INFO_POS, &vPlacePos);
	vPlaceScale = pPlaceTransform->Get_Scale();
	vItemScale = pItemTransform->Get_Scale();

	pItemTransform->Set_Pos(vPlacePos.x, vPlacePos.y + vPlaceScale.y * 0.2f + vItemScale.y * 0.2f, vPlacePos.z);
	dynamic_cast<CInteract*>(pItem)->Set_Ground(true);

	m_bFull = true;
	m_pPlacedItem = pItem;

	PlaySound_PutDown();

	return true;
}

_bool CSinkStation::Get_CanPlace(CGameObject* pItem)
{
	// 오염된 접시만
	CInteract* pInteract = dynamic_cast<CInteract*>(pItem);
	if (nullptr == pInteract)
		return false;

	CInteract::INTERACTTYPE eType = pInteract->Get_InteractType();
	if (CInteract::PLATE == eType)
		if(CPlate::DIRTY == dynamic_cast<CPlate*>(pInteract)->Get_State())
			return true;

	return false;
}

CGameObject* CSinkStation::Get_PlacedItem()
{
	if (nullptr == m_pPlacedItem)
		return nullptr;

	if (0.f < m_fProgress)
		return nullptr;

	dynamic_cast<CInteract*>(m_pPlacedItem)->Set_Ground(false);
	CGameObject* pItem = m_pPlacedItem;

	Set_Empty();

	PlaySound_PickUp();

	return pItem;
}

void CSinkStation::Set_Empty()
{
	m_bFull = false;
	m_pPlacedItem = nullptr;
}

_bool CSinkStation::Enter_Process()
{
	if (!m_bFull)
		return false;

	//m_fProgress = 0.f;
	Set_Process(true);
	m_bProgressVisible = true;

	return true;
}

void CSinkStation::Update_Process(const _float& fTimeDelta)
{
	if (Get_Process())
		Add_Progress(fTimeDelta, 0.5f);
}

void CSinkStation::Exit_Process()
{
	if (!m_bFull)
		return;

	if (!Get_Process())
		return;

	CPlate* pPlate = dynamic_cast<CPlate*>(Get_Item());
	if (!pPlate)
		return;

	if (Get_Progress() >= 1.f)
	{
		Set_Progress(1.f);
		Set_Process(false);
		pPlate->Set_State(CPlate::CLEAN);

		CGameObject* pStation = CManagement::GetInstance()->Get_GameObject(L"GameObject_Layer", L"Sink_Plate");
		if (!pStation)
			return;
		
		if (IPlace* pPlace = dynamic_cast<IPlace*>(pStation))
			if (pPlace->Set_Place(pPlate, pStation))
				Set_Empty();

		m_bProgressVisible = false;
	}
}

HRESULT CSinkStation::Add_Component()
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

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Sink"));
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

CSinkStation* CSinkStation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSinkStation* pSinkStation = new CSinkStation(pGraphicDev);

	if (FAILED(pSinkStation->Ready_GameObject()))
	{
		Safe_Release(pSinkStation);
		MSG_BOX("Station_Sink Failed");
		return nullptr;
	}

	return pSinkStation;
}

void CSinkStation::Draw_Progress()
{
	if (m_pProgressBack && m_pProgressFill)
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

		dynamic_cast<CUi_CookLodingBox*>(m_pProgressBack)->UpdatePosition(vPos);
		dynamic_cast<CUi_CookLoding*>(m_pProgressFill)->UpdatePosition(vPos);
		dynamic_cast<CUi_CookLoding*>(m_pProgressFill)->Set_Progress(m_fProgress);

		dynamic_cast<CUi_CookLodingBox*>(m_pProgressBack)->On_Off(m_bProgressVisible);
		dynamic_cast<CUi_CookLoding*>(m_pProgressFill)->On_Off(m_bProgressVisible);
	}
	else if (!m_pProgressBack)
	{
		CGameObject* pProgressBack = CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_Object10");

		if (!pProgressBack)
			return;

		m_pProgressBack = dynamic_cast<CUi_CookLodingBox*>(pProgressBack)->Make_cookLodingBox(true);
	}
	else if (!m_pProgressFill)
	{
		CGameObject* pProgressFill = CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_Object11");

		if (!pProgressFill)
			return;

		m_pProgressFill = dynamic_cast<CUi_CookLoding*>(pProgressFill)->Make_cookLoding(true, m_pProgressBack);
	}
}

void CSinkStation::Free()
{
	CInteract::Free();
}