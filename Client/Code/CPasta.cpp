#include "pch.h"
#include "CPasta.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "IState.h"
#include "CManagement.h"
#include "CUi_BurntFood.h"

CPasta::CPasta(LPDIRECT3DDEVICE9 pGraphicDev)
	: CIngredient(pGraphicDev)
{
}

CPasta::CPasta(const CGameObject& rhs)
	: CIngredient(rhs)
{
}

CPasta::~CPasta()
{
}

HRESULT CPasta::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_eIngredientType = PASTA;
	m_eCookState = RAW;
	m_pCurrentState = new IRawState();

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = true;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bApplyKnockBack = true;

	return S_OK;
}

_int CPasta::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	Draw_Icon();
	Draw_BurntIcon();

	if (m_pCurrentState)
		m_pCurrentState->Update_State(this, fTimeDelta);

	_matrix matWorld;
	m_pTransformCom->Get_World(&matWorld);
	Billboard(matWorld);
	m_pTransformCom->Set_World(&matWorld);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this); 

	return iExit;
}

void CPasta::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Engine::CGameObject::Compute_ViewZ(&vPos);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPasta::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int i = 0; i < (int)m_bHighlight + 1; ++i)
	{
		if (m_vecTextureCom.size() > i && m_vecTextureCom[i])
		{
			int iIndex = 0;
			switch (m_eCookState)
			{
			case COOKED: iIndex = 1; break;
			case DONE: iIndex = 2; break;
			case BURNT: iIndex = 3; break;
			}

			m_vecTextureCom[i]->Set_Texture(iIndex);
			if (FAILED(Set_Material()))
				return;
			m_pBufferCom->Render_Buffer();
		}
	}

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CPasta::Reset()
{
	CIngredient::Reset();
	m_bBurntIconVisible = false;
	dynamic_cast<CUi_BurntFood*>(m_pBurntIcon)->On_Off(m_bBurntIconVisible);
}

HRESULT CPasta::Add_Component()
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

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_IngredientTexture_Pasta"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_IngredientTexture_Pasta_Alpha"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture_Alpha", pComponent });

	return S_OK;
}

void CPasta::Draw_BurntIcon()
{
	if (!m_pBurntIcon)
	{
		CGameObject* pObj = CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_BurntFood");
		if (!pObj)
			return;

		m_pBurntIcon = dynamic_cast<CUi_BurntFood*>(pObj)->Make_BurntFood(true);

		CTransform* pTransform = dynamic_cast<CTransform*>(m_pBurntIcon->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Transform"));
		_vec3 vPos{};
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
	}
	else
	{
		if (!m_bBurntIconVisible && m_eCookState == BURNT)
		{
			m_bBurntIconVisible = true;
			m_bIconVisible = false;
		}

		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		dynamic_cast<CUi_BurntFood*>(m_pBurntIcon)->UpdatePosition(vPos);
		dynamic_cast<CUi_BurntFood*>(m_pBurntIcon)->On_Off(m_bBurntIconVisible);
	}
}

CPasta* CPasta::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPasta* pPasta = new CPasta(pGraphicDev);

	if (FAILED(pPasta->Ready_GameObject()))
	{
		Safe_Release(pPasta);
		MSG_BOX("Ingredient_Pasta Create Failed");
		return nullptr;
	}

	return pPasta;
}

void CPasta::Free()
{
	CIngredient::Free();
}