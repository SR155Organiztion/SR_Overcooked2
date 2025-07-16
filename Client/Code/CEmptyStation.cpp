#include "pch.h"
#include "CEmptyStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CInteractMgr.h"
#include "CIngredient.h"

#include "CFontMgr.h"

CEmptyStation::CEmptyStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev), IShadow(pGraphicDev)
{

}

CEmptyStation::CEmptyStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CEmptyStation::~CEmptyStation()
{
}

HRESULT CEmptyStation::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale({ 1.f, 0.5f, 1.f });
	m_pTransformCom->Set_Pos(4.5f, m_pTransformCom->Get_Scale().y * 0.5f, 8.f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bIsStation = true;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);	// 삭제 예정

	return S_OK;
}

_int CEmptyStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);
	
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	//swprintf_s(m_szProgress, L"%d, %p", m_bFull, m_pPlacedItem);	// 디버깅

	return iExit;
}

void CEmptyStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Engine::CGameObject::Compute_ViewZ(&vPos);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CEmptyStation::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

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

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	//_vec2   vPos{ 100.f, 200.f };
	//CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szProgress, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));	// 디버깅
}

_bool CEmptyStation::Get_CanPlace(CGameObject* pItem)
{
	// 모든 재료 / 도구(냄비,후라이팬,접시) / 소화기
	return true;
}

HRESULT CEmptyStation::Add_Component()
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

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Empty"));
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

void CEmptyStation::On_Collision(CGameObject* _pGameObject) {
	

	if (dynamic_cast<CIngredient*>(_pGameObject)) {
		_pGameObject = _pGameObject;
	}
}

_bool CEmptyStation::On_Snap(CGameObject* _pGameObject)
{
	if (dynamic_cast<CIngredient*>(_pGameObject)) {
		if (m_bFull) {
			IPlace* pTool = dynamic_cast<IPlace*>(m_pPlacedItem);
			if (pTool) {
				if (pTool->Set_Place(_pGameObject, m_pPlacedItem)) {
					dynamic_cast<CIngredient*>(_pGameObject)->Set_Ground(true);
					return true;
				}
			}
			return false;
		}
		else {
			Set_Place(_pGameObject, this);
			dynamic_cast<CIngredient*>(_pGameObject)->Set_Ground(true);
			return true;
		}
	}
	return false;
}

CEmptyStation* CEmptyStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) 
{
	CEmptyStation* pEmptyStation = new CEmptyStation(pGraphicDev);

	if (FAILED(pEmptyStation->Ready_GameObject()))
	{
		Safe_Release(pEmptyStation);
		MSG_BOX("Station_Empty Failed");
		return nullptr;
	}

	return pEmptyStation;
}

void CEmptyStation::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);	// 삭제 예정
	Engine::CGameObject::Free();
}