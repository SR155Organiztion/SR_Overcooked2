#include "pch.h"
#include "CTestEffect.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CTestEffect::CTestEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CTestEffect::CTestEffect(const CEffect& rhs)
	: CEffect(rhs)
{
}

CTestEffect::~CTestEffect()
{
}

HRESULT CTestEffect::Ready_Effect()
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	m_fFrame = 0.f;


	return S_OK;
}

HRESULT	CTestEffect::Add_Component()
{
	CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_TestEffect"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CTestEffect::Play_Effect(_vec3 StartPos)
{
	m_bActive = true;
	m_fFrame = 0.f;
	m_pTransformCom->Set_Pos(StartPos.x, StartPos.y - 0.5f, StartPos.z);

}

_int CTestEffect::Update_Effect(const _float& fTimeDelta)
{
	CEffect::Update_Effect(fTimeDelta);

	m_fFrame += 90.f * fTimeDelta;

	if (90.f < m_fFrame) {
		//m_fFrame = 0.f;
		m_bActive = false;
		return 0;
	}

	//ºôº¸µå Àû¿ë
	_matrix matWorld, matView, matBill;

	m_pTransformCom->Get_World(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);
	D3DXMatrixInverse(&matView, 0, &matView);

	_vec3	vViewScale, vViewTrans;
	D3DXQUATERNION qViewRot;
	D3DXMatrixDecompose(&vViewScale, &qViewRot, &vViewTrans, &matView);
	_matrix matViewRot;  D3DXMatrixRotationQuaternion(&matViewRot, &qViewRot);

	matWorld = matViewRot * matWorld;

	m_pTransformCom->Set_World(&matWorld);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return S_OK;
}

void CTestEffect::LateUpdate_Effect(const _float& fTimeDelta)
{
}

void CTestEffect::Render_GameObject()
{
	if (!m_bActive) return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_int)m_fFrame);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

CEffect* CTestEffect::Clone()
{
	CEffect* pTestEffect = new CTestEffect(*this);
	if (FAILED(pTestEffect->Ready_Effect())) {
		Safe_Release(pTestEffect);
		return nullptr;
	}

	
	return pTestEffect;
}

CEffect* CTestEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev) 
{
	CTestEffect* pEffect = new CTestEffect(pGraphicDev);

	if (!pEffect)
	{
		Safe_Release(pEffect);
		MSG_BOX("Test Effect Create Failed");
		return nullptr;
	}

	return pEffect;
}

void CTestEffect::Free()
{
	Engine::CEffect::Free();
}
