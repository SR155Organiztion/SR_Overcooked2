#include "pch.h"
#include "CChopStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CInteractMgr.h"

CChopStation::CChopStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CChopStation::CChopStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CChopStation::~CChopStation()
{
}

HRESULT CChopStation::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(2.5f, m_pTransformCom->Get_Scale().y * 0.5f, 8.f);

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);

	return S_OK;
}

_int CChopStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	Update_Process(fTimeDelta);
	Exit_Process();

	//if (GetAsyncKeyState('C'))
	//	Enter_Process();
	//if (GetAsyncKeyState('X'))
	//	Pause_Process();

	return iExit;
}

void CChopStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CChopStation::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

_bool CChopStation::Enter_Process()
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient || CIngredient::RAW != pIngredient->Get_State())
		return false;

	Set_Process(true);
	pIngredient->Set_Lock(true);

	return true;
}

void CChopStation::Update_Process(const _float& fTimeDelta)
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient || CIngredient::CHOPPED == pIngredient->Get_State() || CIngredient::DONE == pIngredient->Get_State())
		return;
	
	if (Get_Process())
		Add_Progress(fTimeDelta, 0.5f);
}

void CChopStation::Exit_Process()
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient)
		return;

	if (1.f <= Get_Progress())
	{
		pIngredient->Set_State(CIngredient::CHOPPED);
		pIngredient->Set_Lock(false);
		Set_Progress(0.f);
	}
}

HRESULT CChopStation::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Chop"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CChopStation* CChopStation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CChopStation* pChopStation = new CChopStation(pGraphicDev);

	if (FAILED(pChopStation->Ready_GameObject()))
	{
		Safe_Release(pChopStation);
		MSG_BOX("Station_Chop Failed");
		return nullptr;
	}

	return pChopStation;
}

void CChopStation::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);
	Engine::CGameObject::Free();
}

_bool CChopStation::Get_CanPlace(CGameObject* pItem)
{
	// 모든 재료 / 도구 / 소화기
	return true;
}