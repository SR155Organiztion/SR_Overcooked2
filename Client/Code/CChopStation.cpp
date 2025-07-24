#include "pch.h"
#include "CChopStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "IState.h"
#include "CUi_CookLoding.h"
#include "CManagement.h"

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

	m_pTransformCom->Set_Scale({ 1.f, 0.5f, 1.f });

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bIsStation = true;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	return S_OK;
}

_int CChopStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	Update_Process(fTimeDelta);
	Exit_Process();

	Draw_Progress();

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CChopStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Engine::CGameObject::Compute_ViewZ(&vPos);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CChopStation::Render_GameObject()
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

_bool CChopStation::Enter_Process()
{
	// m_bFull이고, 아이템이 있고, 아이템이 RAW 상태 일 때
	if (!m_bFull)
		return false;

	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient || CIngredient::RAW != pIngredient->Get_State())
		return false;

	// 토마토, 양상추, 오이, 생선, 새우일 때
	CIngredient::INGREDIENT_TYPE eType = pIngredient->Get_IngredientType();
	switch (eType)
	{
	case CIngredient::TOMATO:
	case CIngredient::LETTUCE:
	case CIngredient::CUCUMBER:
	case CIngredient::FISH:
	case CIngredient::SHRIMP:
	case CIngredient::TOMATOSOUP:
		Set_Process(true);
		pIngredient->Set_Lock(true);
		m_bProgressVisible = true;
		return true;
	}

	return false;
}

void CChopStation::Update_Process(const _float& fTimeDelta)
{
	if (Get_Process())
		Add_Progress(fTimeDelta, 0.5f);
}

void CChopStation::Exit_Process()
{
	if (!m_bFull)
		return;

	CIngredient* pIngredient = dynamic_cast<CIngredient*>(Get_Item());
	if (nullptr == pIngredient)
		return;

	if (!Get_Process())
		return;

	if (Get_Progress() >= 1.f)
	{
		Set_Progress(1.f);
		Set_Process(false);
		pIngredient->ChangeState(new IChopState());
		pIngredient->Set_Lock(false);
		m_bProgressVisible = false;
	}
}

_bool CChopStation::Get_CanPlace(CGameObject* pItem)
{
	// 모든 재료 / 도구(냄비, 후라이팬, 접시) / 소화기
	return true;
}

_bool CChopStation::On_Snap(CGameObject* _pGameObject)
{
	if (dynamic_cast<CIngredient*>(_pGameObject)) {
		_vec3 vObjectPos;
		dynamic_cast<CTransform*>(_pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vObjectPos);
		_float thisY = m_pTransformCom->Get_Scale().y;
		if (thisY * 0.8f > vObjectPos.y * 0.5f) {
			return false;
		}

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

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Chop"));
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

void CChopStation::Draw_Progress()
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
	CInteract::Free();
}