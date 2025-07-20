#include "pch.h"
#include "CAnyEffect.h"
#include "CProtoMgr.h"
#include "CRenderer.h"


CAnyEffect::CAnyEffect(LPDIRECT3DDEVICE9 pGraphicDev)
    : CEffect(pGraphicDev)
{
}

CAnyEffect::CAnyEffect(const CAnyEffect& rhs)
    : CEffect(rhs)
{
	m_szTextureKey = rhs.m_szTextureKey;
	m_iCount = rhs.m_iCount;
	m_fAccel = rhs.m_fAccel;
	m_vEffectPos = rhs.m_vEffectPos;
	m_vEffectScale = rhs.m_vEffectScale;
}

CAnyEffect::~CAnyEffect()
{
}

HRESULT CAnyEffect::Ready_Effect()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(m_vEffectScale);

    return S_OK;
}

_int CAnyEffect::Update_Effect(const _float& fTimeDelta)
{
    Engine::CEffect::Update_Effect(fTimeDelta);

	m_fFrame += m_iCount * fTimeDelta * m_fAccel;

	if (m_iCount < m_fFrame) {
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

	Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return S_OK;
}

void CAnyEffect::LateUpdate_Effect(const _float& fTimeDelta)
{
    Engine::CEffect::LateUpdate_Effect(fTimeDelta);

}

void CAnyEffect::Render_GameObject()
{
	if (!m_bActive) return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_int)m_fFrame);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(128, 255, 255, 255));

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CAnyEffect::Add_Component()
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(m_szTextureKey));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    return S_OK;
}

void CAnyEffect::Play_Effect(_vec3 StartPos)
{
	m_bActive = true;
	m_fFrame = 0.f;
	_vec3 vResult = StartPos + m_vEffectPos;
	m_pTransformCom->Set_Pos(vResult.x, vResult.y, vResult.z);
}

CEffect* CAnyEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* ProtoTextureName, _int Count, 
	_float fAccel, _vec3 vRenderPos, _vec3 vScale)
{
	CAnyEffect* pEffect = new CAnyEffect(pGraphicDev);

	if (!pEffect) {
		Safe_Release(pEffect);
		MSG_BOX("AnyEffect Create Failed");
		return nullptr;
	}

	pEffect->m_szTextureKey = ProtoTextureName;
	pEffect->m_iCount = Count;
	pEffect->m_fAccel = fAccel;
	pEffect->m_vEffectPos = vRenderPos;
	pEffect->m_vEffectScale = vScale;

	return pEffect;
}

CEffect* CAnyEffect::Clone()
{
	CEffect* pEffect = new CAnyEffect(*this);
	if (FAILED(pEffect->Ready_Effect())) {
		Safe_Release(pEffect);
		MSG_BOX("AnyEffect Create Clone Failed");
		return nullptr;
	}
	return pEffect;
}

void CAnyEffect::Free()
{
    Engine::CEffect::Free();
}
