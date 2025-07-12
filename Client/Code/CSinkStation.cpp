#include "pch.h"
#include "CSinkStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CPlate.h"
#include "CManagement.h"

#include "CInteractMgr.h"
#include "CFontMgr.h"

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
	m_pTransformCom->Set_Pos(7.5f, m_pTransformCom->Get_Scale().y, 4.5f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);	// 삭제 예정

	return S_OK;
}

_int CSinkStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	Update_Process(fTimeDelta);
	Exit_Process();

	if (GetAsyncKeyState('C'))
		Enter_Process();
	if (GetAsyncKeyState('X'))
		Pause_Process();

	swprintf_s(m_szTemp, L"SinkStation %f", m_fProgress);	// 디버깅

	return iExit;
}

void CSinkStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CSinkStation::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	int iIndex = (m_bProcess) ? 1 : 0;

	m_pTextureCom->Set_Texture(iIndex);

	if (FAILED(Set_Material()))
		return;

	m_pBufferCom->Render_Buffer();

	//_vec2   vPos{ 100.f, 300.f };
	//CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szTemp, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));	// 디버깅
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

_bool CSinkStation::Enter_Process()
{
	if (!m_bFull)
		return false;

	Set_Process(true);

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
		pPlate->Set_Clean();

		CGameObject* pStation = CManagement::GetInstance()->Get_GameObject(L"GameObject_Layer", L"Station_CleanPlate");
		if (!pStation)
			return;
		
		if (IPlace* pPlace = dynamic_cast<IPlace*>(pStation))
			pPlace->Set_Place(pPlate, pStation);
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Sink"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

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

void CSinkStation::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);
	Engine::CGameObject::Free();
}