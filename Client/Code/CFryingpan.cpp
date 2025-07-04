#include "pch.h"
#include "CFryingpan.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "IState.h"
#include "CFontMgr.h"
#include "CInteractMgr.h"

CFryingpan::CFryingpan(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CFryingpan::CFryingpan(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CFryingpan::~CFryingpan()
{
}

HRESULT CFryingpan::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(3.f, m_pTransformCom->Get_Scale().y, 4.f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bApplyKnockBack = true;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::TOOL, this);

	return S_OK;
}

_int CFryingpan::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CFryingpan::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Update_ContentPosition(this, Get_Item());
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CFryingpan::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CFryingpan::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ToolTexture_Fryingpan"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

_bool CFryingpan::Enter_Process()
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient || CIngredient::BURNT == pIngredient->Get_State())
		return false;

	Set_Process(true);
	pIngredient->Set_State(CIngredient::COOKED);
	pIngredient->Set_Lock(true);

	return true;
}

void CFryingpan::Update_Process(const _float& fTimeDelta)
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient || CIngredient::BURNT == pIngredient->Get_State())
		return;

	if (Get_Process())
		Add_Progress(fTimeDelta, 0.2f);
}

void CFryingpan::Exit_Process()
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

_bool CFryingpan::Set_Place(CGameObject* pItem, CGameObject* pPlace)
{
	if (IPlace::Set_Place(pItem, pPlace))
	{
		//CTransform* pTransform = dynamic_cast<CTransform*>(pItem->Get_Component(ID_DYNAMIC, L"Com_Transform"));	// Set_Ground에서 처리
		//pTransform->Rotation(ROT_Z, -pTransform->m_vAngle[2]);
		dynamic_cast<CInteract*>(pItem)->Set_Collision(false);
	}

	return true;
}

_bool CFryingpan::Get_CanPlace(CGameObject* pItem)
{
	// 재료 (CHOP 상태의 토마토만)
	//CInteract* pInteract = dynamic_cast<CInteract*>(pItem);
	//
	//if (nullptr == pInteract)
	//	return false;
	//
	//if (CInteract::INGREDIENT == pInteract->Get_InteractType())
	//{
	//	CIngredient* pIngredient = dynamic_cast<CIngredient*>(pInteract);
	//	if (nullptr == pIngredient)
	//		return false;
	//
	//	if (CIngredient::RICE == pIngredient->Get_Type() || CIngredient::PASTA == pIngredient->Get_Type())
	//		if (CIngredient::RAW == pIngredient->Get_State())
	//			return true;
	//}

	return true;

	return false;
}

_bool CFryingpan::Get_CanCarry() const
{
	return _bool();
}

CFryingpan* CFryingpan::Create(LPDIRECT3DDEVICE9 pGraphicDev) 
{
	CFryingpan* pFryingpan = new CFryingpan(pGraphicDev);

	if (FAILED(pFryingpan->Ready_GameObject()))
	{
		Safe_Release(pFryingpan);
		MSG_BOX("Fryingpan Create Failed");
		return nullptr;
	}

	return pFryingpan;
}

void CFryingpan::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::TOOL, this);
	CInteract::Free();
}