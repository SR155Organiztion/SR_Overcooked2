#include "pch.h"
#include "CTrashStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CObjectPoolMgr.h"
#include "CPlate.h"
#include "CInteractMgr.h"

CTrashStation::CTrashStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CTrashStation::CTrashStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CTrashStation::~CTrashStation()
{
}

HRESULT CTrashStation::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale({ 1.f, 0.5f, 1.f });
	m_pTransformCom->Set_Pos(12.5f, m_pTransformCom->Get_Scale().y * 0.5f, 6.5f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);		// ���� ����

	return S_OK;
}

_int CTrashStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CTrashStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTrashStation::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	if (FAILED(Set_Material()))
		return;

	m_pBufferCom->Render_Buffer();
}

_bool CTrashStation::Set_Place(CGameObject* pItem, CGameObject* pPlace)
{
	// nullptr �˻�
	if (!pItem || !pPlace)
		return false;

	if (!Get_CanPlace(pItem))
		return false;

	CInteract* pInteract = dynamic_cast<CInteract*>(pItem);
	if (!pInteract)
		return false;

	CInteract::INTERACTTYPE eInteractType = pInteract->Get_InteractType();

	if (CInteract::INGREDIENT == eInteractType)
	{	
		// ����� ��� ObjectPool�� ��ȯ
		CObjectPoolMgr::GetInstance()->Return_Object(pInteract->Get_SelfId(), pInteract);
		CManagement::GetInstance()->Delete_GameObject(L"GameObject_Layer", pInteract->Get_SelfId(), pItem);
	}
	else if (CInteract::PLATE == eInteractType)
	{
		// ������ ��� �ȿ� �ִ� ���빰 ����
		// TODO : ������� �ϴµ� ���� �ٽ� �� ���� �ذ��ؾ�
		dynamic_cast<CPlate*>(pItem)->Clear_Plate();
	}

	return true;
}

_bool CTrashStation::Get_CanPlace(CGameObject* pItem)
{ 
	CInteract* pInteract = dynamic_cast<CInteract*>(pItem);
	if (!pInteract)
		return false;

	// ����� �Ǵ�
	if (CInteract::INGREDIENT == pInteract->Get_InteractType())
		return true;

	// ����ᰡ ��� ����
	if (CInteract::PLATE == pInteract->Get_InteractType())
		return true;

	return false;
}

HRESULT CTrashStation::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Trash"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CTrashStation* CTrashStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) 
{
	CTrashStation* pTrashStation = new CTrashStation(pGraphicDev);

	if (FAILED(pTrashStation->Ready_GameObject()))
	{
		Safe_Release(pTrashStation);
		MSG_BOX("Station_Trash Failed");
		return nullptr;
	}

	return pTrashStation;
}

void CTrashStation::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);		// ���� ����
	Engine::CGameObject::Free();
}