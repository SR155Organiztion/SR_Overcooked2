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

	m_pTransformCom->Set_Pos(7.f, m_pTransformCom->Get_Scale().y, 3.f);

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::TOOL, this);

	return S_OK;
}

_int CPot::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	Update_Process(fTimeDelta);
	Exit_Process();

	swprintf_s(m_szTemp, L"%d\n%f", m_bProcess, m_fProgress);

	return iExit;
}

void CPot::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Update_ContentPosition(this, Get_Item());

	//// IPlace 테스트
	if (GetAsyncKeyState('O'))
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

		dynamic_cast<CTransform*>(pObj->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(4.f, m_pTransformCom->Get_Scale().y * 0.5f, 6.f);
	}

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPot::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	_vec2   vPos{ 100.f, 200.f };
	CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szTemp, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
}

_bool CPot::Enter_Process()
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient|| CIngredient::BURNT == pIngredient->Get_State())
		return false;

	Set_Process(true); 
	pIngredient->Set_State(CIngredient::COOKED);
	pIngredient->Set_Lock(true);

	return true;
}

void CPot::Update_Process(const _float& fTimeDelta)
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient || CIngredient::BURNT == pIngredient->Get_State())
		return;

	if (Get_Process())
		Add_Progress(fTimeDelta, 0.2f);		 
}

void CPot::Exit_Process()
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient)
		return;

	if (2.f <= Get_Progress())
	{
		pIngredient->Set_State(CIngredient::BURNT);
		Set_Progress(0.f);
		Set_Process(false); 
	}
	else if (1.f <= Get_Progress())
	{		 
		pIngredient->Set_State(CIngredient::DONE);
		//pIngredient->Set_Lock(false);
	}
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
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::TOOL, this);
	CInteract::Free();
}

_bool CPot::Get_CanPlace(CGameObject* pItem)
{
	// 재료 (RAW 상태의 쌀, 파스타만)
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(pItem);
	if (nullptr == pIngredient)
		return false;

	if (CIngredient::RICE == pIngredient->Get_Type() || CIngredient::PASTA == pIngredient->Get_Type())
		if (CIngredient::RAW == pIngredient->Get_State())
			return true;

	return false;
}

_bool CPot::Get_CanCarry() const
{
	return _bool();
}