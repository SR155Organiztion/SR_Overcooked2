#include "pch.h"
#include "CFireEffect.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CFireEffect::CFireEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CEffect(pGraphicDev), m_fFrame(0.f)
{
}

CFireEffect::CFireEffect(const CEffect& rhs)
	: CEffect(rhs), m_fFrame(0.f)
{
}

CFireEffect::~CFireEffect()
{
}


HRESULT CFireEffect::Ready_Effect()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale({6.f, 6.f, 6.f });

	return S_OK;
}

HRESULT	CFireEffect::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_FireEffect"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CFireEffect::Play_Effect(_vec3 StartPos)
{
	m_bActive = true;
	m_fFrame = 0.f;
	_vec3 vScale{};
	vScale = m_pTransformCom->Get_Scale();
	m_pTransformCom->Set_Pos(StartPos.x, StartPos.y + 0.25f + vScale.y * 0.46f, StartPos.z - 0.25f);
}

_int CFireEffect::Update_Effect(const _float& fTimeDelta)
{
	int iExit = CEffect::Update_Effect(fTimeDelta);

	m_fFrame += 32.f * fTimeDelta;

	if (32.f < m_fFrame) {
		//m_fFrame = 0.f;
		m_bActive = false;
		return 0;
	}

	_matrix		matWorld, matView, matBill;
	m_pTransformCom->Get_World(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;
	D3DXMatrixInverse(&matBill, 0, &matBill);
	matWorld = matBill * matWorld;
	m_pTransformCom->Set_World(&matWorld);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CFireEffect::LateUpdate_Effect(const _float& fTimeDelta)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Engine::CGameObject::Compute_ViewZ(&vPos);
}

void CFireEffect::Render_GameObject()
{
	if (!m_bActive) 
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_int)m_fFrame);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

CFireEffect* CFireEffect::Clone()
{
	CFireEffect* pFireEffect = new CFireEffect(*this);

	if (FAILED(pFireEffect->Ready_Effect())) 
	{
		Safe_Release(pFireEffect);
		MSG_BOX("Fire Effect Clone Create Failed");
		return nullptr;
	}

	return pFireEffect;
}

CFireEffect* CFireEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFireEffect* pFireEffect = new CFireEffect(pGraphicDev);

	if (FAILED(pFireEffect->Ready_Effect()))
	{
		Safe_Release(pFireEffect);
		MSG_BOX("Fire Effect Create Failed");
		return nullptr;
	}

	return pFireEffect;
}

void CFireEffect::Free()
{
	Engine::CEffect::Free();
}