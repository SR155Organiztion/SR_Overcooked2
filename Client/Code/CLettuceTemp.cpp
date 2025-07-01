#include "pch.h"
#include "CLettuceTemp.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "IState.h"
#include "CFontMgr.h"

CLettuceTemp::CLettuceTemp(LPDIRECT3DDEVICE9 pGraphicDev)
	: CIngredient(pGraphicDev)
{
}

CLettuceTemp::CLettuceTemp(const CGameObject& rhs)
	: CIngredient(rhs)
{
}

CLettuceTemp::~CLettuceTemp()
{
}

HRESULT CLettuceTemp::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_eType = LETTUCE;
	m_pCurrentState = new IRawState();
	m_pTransformCom->Set_Pos(2.f, m_pTransformCom->Get_Scale().y, 2.f);

	return S_OK;
}

_int CLettuceTemp::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CLettuceTemp::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CLettuceTemp::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	int iIndex = 0;

	switch (m_eCookState)
	{
	case RAW:
		iIndex = 0;
		break;
	case CHOPPED:
	case DONE:
		iIndex = 1;
		break;
	}

	m_pTextureCom->Set_Texture(iIndex);

	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CLettuceTemp::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_IngredientTexture_Lettuce"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CLettuceTemp* CLettuceTemp::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLettuceTemp* pLettuce = new CLettuceTemp(pGraphicDev);

	if (FAILED(pLettuce->Ready_GameObject()))
	{
		Safe_Release(pLettuce);
		MSG_BOX("Lettuce Create Failed");
		return nullptr;
	}

	return pLettuce;
}

void CLettuceTemp::Free()
{
	CIngredient::Free();
}