#include "pch.h"
#include "CInvisibleStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"

CInvisibleStation::CInvisibleStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CInvisibleStation::CInvisibleStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CInvisibleStation::~CInvisibleStation()
{
}

HRESULT CInvisibleStation::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_stOpt.bApplyGravity = false;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	return S_OK;
}

_int CInvisibleStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_I) & 0x80)
	{
		if (m_bKeyCheck) return 0;
		m_bKeyCheck = true;
		//--------------- Body ---------------//

		m_bVisible = !m_bVisible;
	}
	else m_bKeyCheck = false;

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CInvisibleStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CInvisibleStation::Render_GameObject()
{
	if (m_bVisible)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

		m_pTextureCom->Set_Texture(0);

		//if (FAILED(Set_Material()))
		//	return;

		m_pBufferCom->Render_Buffer();
	} 
}

_bool CInvisibleStation::Get_CanPlace(CGameObject* pItem)
{
	return false;
}

HRESULT CInvisibleStation::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Invisible"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CInvisibleStation* CInvisibleStation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInvisibleStation* pInvisibleStation = new CInvisibleStation(pGraphicDev);

	if (FAILED(pInvisibleStation->Ready_GameObject()))
	{
		Safe_Release(pInvisibleStation);
		MSG_BOX("Station_Invisible Failed");
		return nullptr;
	}

	return pInvisibleStation;
}

void CInvisibleStation::Free()
{
	CInteract::Free();
}