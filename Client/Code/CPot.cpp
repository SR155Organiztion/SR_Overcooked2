#include "pch.h"
#include "CPot.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "IState.h"
#include "CFontMgr.h"
#include "CInteractMgr.h"
#include "CIngredient.h"

CPot::CPot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CPot::CPot(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CPot::~CPot()
{
}

HRESULT CPot::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(-2.f, m_pTransformCom->Get_Scale().y, 2.f);

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::CARRY, this);

	return S_OK;
}

_int CPot::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CPot::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

	// IPlace 테스트
	if (GetAsyncKeyState('L'))
	{
		list<CGameObject*>* pListStation = CInteractMgr::GetInstance()->Get_List(CInteractMgr::STATION);
		CGameObject* pStation = nullptr;

		if (pListStation)
			pStation = pListStation->front();

		if (pStation)
			dynamic_cast<IPlace*>(pStation)->Set_Place(this, pStation);
	}
	//
	if (GetAsyncKeyState('K'))
	{
		list<CGameObject*>* pListStation = CInteractMgr::GetInstance()->Get_List(CInteractMgr::STATION);
		CGameObject* pStation = nullptr;

		if (pListStation)
			pStation = pListStation->front();

		CGameObject* pObj = nullptr;

		if (pStation)
			pObj = dynamic_cast<IPlace*>(pStation)->Get_PlacedItem();

		if (nullptr == pObj)
			return;

		dynamic_cast<IPlace*>(pStation)->Set_Empty();

		dynamic_cast<CTransform*>(pObj->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(-2.f, 0.f, 0.f);
	}
}

void CPot::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CPot::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ToolTexture_Pot"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CPot* CPot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPot* pPot = new CPot(pGraphicDev);

	if (FAILED(pPot->Ready_GameObject()))
	{
		Safe_Release(pPot);
		MSG_BOX("Pot Create Failed");
		return nullptr;
	}

	return pPot;
}

void CPot::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::CARRY, this);
	CInteract::Free();
}

_bool CPot::Get_CanPlace(CGameObject* pItem)
{
	// 재료 (RAW 상태의 쌀, 파스타만)
	CInteract* pInteract = dynamic_cast<CInteract*>(pItem);

	if (nullptr == pInteract)
		return false;

	if (CInteract::INGREDIENT == pInteract->Get_InteractType())
	{
		CIngredient* pIngredient = dynamic_cast<CIngredient*>(pInteract);
		if (nullptr == pIngredient)
			return false;

		if(CIngredient::RICE == pIngredient->Get_Type() || CIngredient::PASTA == pIngredient->Get_Type())
			if (CIngredient::RAW == pIngredient->Get_State())
				return true;
	}

	return false;
}

_bool CPot::CanCook(CIngredient* pIngredient) const
{
	return _bool();
}

void CPot::Cook(CIngredient* pIngredient)
{
}

_bool CPot::Get_CanCarry() const
{
	return _bool();
}