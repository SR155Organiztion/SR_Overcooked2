#include "pch.h"
#include "CExtinguishEffect.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CRealPlayer.h"
#include "CFireExtinguisher.h"

CExtinguishEffect::CExtinguishEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CExtinguishEffect::CExtinguishEffect(const CEffect& rhs)
	:  CEffect(rhs)
{
}

CExtinguishEffect::~CExtinguishEffect()
{
}

HRESULT CExtinguishEffect::Ready_Effect()
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	m_fFrame = 0.f;
	m_fMove = 0.f;

	return S_OK;
}

_int CExtinguishEffect::Update_Effect(const _float& fTimeDelta)
{
	CEffect::Update_Effect(fTimeDelta);


	if (2.f > m_fFrame) {
		m_fFrame += 9.f * fTimeDelta;
	}
	else if (2.f < m_fFrame) {
		m_fFrame += 8.f * fTimeDelta;
	}
	
	m_fScale += 5.f * fTimeDelta;
	//if (m_fScale > 1.5f) {
	//	m_fScale = 1.f;
	//}
	m_pTransformCom->Set_Scale({ m_fScale ,m_fScale ,1.f });

	if (2.5f < m_fMove) {
		m_bActive = false;
		return 0;
	}

	m_pTransformCom->Move_Pos(&m_vDir, 5.f, fTimeDelta);
	m_fMove += 5.f * fTimeDelta;
	

	//빌보드 적용
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

void CExtinguishEffect::LateUpdate_Effect(const _float& fTimeDelta)
{
}

void CExtinguishEffect::Render_GameObject()
{
	if (!m_bActive) 
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_int)m_fFrame);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT	CExtinguishEffect::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_CloudEffect"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	return S_OK;

}

void CExtinguishEffect::Play_Effect(_vec3 StartPos)
{
	m_bActive = true;
	m_fFrame = 0.f;
	m_fMove = 0.f;
	m_fScale = 0.1f;
	m_pTransformCom->Set_Pos(StartPos.x, StartPos.y + 0.1f, StartPos.z);
	m_pTransformCom->Set_Scale({ m_fScale,m_fScale ,1.f });
	m_vDir = {};
	// 소화기 오브젝트 찾아와서 룩벡터 설정
	CGameObject* pPlayer = nullptr;
	pPlayer = CManagement::GetInstance()->Get_GameObject(L"GameObject_Layer", L"Player1"); 
	if (pPlayer) {
		CFireExtinguisher* pExtinguisher = dynamic_cast<CFireExtinguisher*>(dynamic_cast<CRealPlayer*>(pPlayer)->Get_GrabObj());
		if (pExtinguisher) {
			dynamic_cast<CTransform*>(pPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_LOOK, &m_vDir);
			D3DXVec3Normalize(&m_vDir, &m_vDir);
			return;
		}
	}
	pPlayer = CManagement::GetInstance()->Get_GameObject(L"GameObject_Layer", L"Player2");
	if (pPlayer) {
		CFireExtinguisher* pExtinguisher = dynamic_cast<CFireExtinguisher*>(dynamic_cast<CRealPlayer*>(pPlayer)->Get_GrabObj());
		if (pExtinguisher) {
			dynamic_cast<CTransform*>(pPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_LOOK, &m_vDir);
			D3DXVec3Normalize(&m_vDir, &m_vDir);
			return;
		}
	}

}

CEffect* CExtinguishEffect::Clone()
{
	CEffect* pExtinguishEffect = new CExtinguishEffect(*this);
	if (FAILED(pExtinguishEffect->Ready_Effect())) {
		Safe_Release(pExtinguishEffect);
		return nullptr;
	}

	return pExtinguishEffect;
}

CEffect* CExtinguishEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CExtinguishEffect* pEffect = new CExtinguishEffect(pGraphicDev);

	if (!pEffect)
	{
		Safe_Release(pEffect);
		MSG_BOX("Extinguish Effect Create Failed");
		return nullptr;
	}

	return pEffect;
}

void CExtinguishEffect::Free()
{
	Engine::CEffect::Free();
}
