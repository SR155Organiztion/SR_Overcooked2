#include "pch.h"
#include "CHitEffect.h"
#include "CProtoMgr.h"
#include "CRenderer.h"


CHitEffect::CHitEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CHitEffect::CHitEffect(const CHitEffect& rhs)
	: CEffect(rhs)
{
}

CHitEffect::~CHitEffect()
{
}

HRESULT CHitEffect::Ready_Effect()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec3 vScale{ 0.8f,0.8f,0.8f };
	m_pTransformCom->Set_Scale(vScale);

	return S_OK;
}

_int CHitEffect::Update_Effect(const _float& fTimeDelta)
{
	Set_RandomPos(fTimeDelta);

	Engine::CEffect::Update_Effect(fTimeDelta);

	m_fFrame += 2.f * fTimeDelta * 5.f;

	if (2.f < m_fFrame) {
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

void CHitEffect::LateUpdate_Effect(const _float& fTimeDelta)
{
	Engine::CEffect::LateUpdate_Effect(fTimeDelta);

}

void CHitEffect::Render_GameObject()
{
	if (!m_bActive) return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_int)m_fFrame);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CHitEffect::Add_Component()
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_HitEffect"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CHitEffect::Set_RandomPos(const _float& dt)
{
	if (m_bSetPos)
		return;
	_int randomNumX = dt * 10000000;
	_int randomNumZ = dt * 10000000000;

	_float randX{}, randZ{};

	if (0 == randomNumX % 2) {
		randX = -(randomNumX % 5);
	}
	else {
		randX = randomNumX % 5;
	}

	if (0 == randomNumZ % 2) {
		randZ = -(randomNumZ % 5);
	}
	else {
		randZ = randomNumZ % 5;
	}
	m_pTransformCom->Set_Pos(m_vOwnerPos.x + randX * 0.1f, m_vOwnerPos.y + 0.5f, m_vOwnerPos.z + randZ * 0.1f);
}

void CHitEffect::Play_Effect(_vec3 StartPos)
{
	m_bActive = true;
	m_fFrame = 0.f;

	//srand(unsigned(time(NULL)));
	//_float randx = (rand() % 5) * 0.1f;
	//_float randz = (rand() % 5) * 0.1f;

	//m_pTransformCom->Set_Pos(StartPos.x + randx, StartPos.y + 0.5f, StartPos.z + randz);
	//m_pTransformCom->Set_Pos(StartPos.x, StartPos.y + 0.5f, StartPos.z);
	m_vOwnerPos = StartPos;
}

CEffect* CHitEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHitEffect* pEffect = new CHitEffect(pGraphicDev);

	if (!pEffect) {
		Safe_Release(pEffect);
		MSG_BOX("AnyEffect Create Failed");
		return nullptr;
	}

	return pEffect;
}

CEffect* CHitEffect::Clone()
{
	CEffect* pEffect = new CHitEffect(*this);
	if (FAILED(pEffect->Ready_Effect())) {
		Safe_Release(pEffect);
		MSG_BOX("HitEffect Create Clone Failed");
		return nullptr;
	}
	return pEffect;
}

void CHitEffect::Free()
{
	Engine::CEffect::Free();
}
