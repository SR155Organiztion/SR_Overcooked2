#include "pch.h"
#include "CPlayerHand.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"

#include "CDInputMgr.h" //테스트용

CPlayerHand::CPlayerHand(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_eHand(HAND_END), m_fAngle(0)
{
}

CPlayerHand::CPlayerHand(const CGameObject& rhs)
	: Engine::CGameObject(rhs)
	, m_eHand(HAND_END), m_fAngle(0)
{
}

CPlayerHand::~CPlayerHand()
{
}

HRESULT CPlayerHand::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;
 
	return S_OK;
}

_int CPlayerHand::Update_GameObject(const _float& fTimeDelta)
{
	// 플레이어의 TransformCom이 먼저 돌고 난 후로 들어오기 때문에 Player의 WorldMat 생성됨
	// FSM이 다 돈 이후임
	
	CDInputMgr* pInput = Engine::CDInputMgr::GetInstance();

	if (pInput->Get_DIKeyState(DIK_O) & 0x80) {
		m_fAngle -= fTimeDelta * 5.f;
	}
	if (pInput->Get_DIKeyState(DIK_L) & 0x80) {
		m_fAngle += fTimeDelta * 5.f;
	}
	
	//공전행렬
	_matrix matRevTrans; D3DXMatrixTranslation(&matRevTrans, 0.f, 0.f, 1.5f);
	_matrix matRevRotX; D3DXMatrixRotationX(&matRevRotX, m_fAngle);
	_matrix matRev = matRevTrans * matRevRotX;
	//부모월드행렬
	_matrix matPlayerWorld;
	m_pPlayerTransformCom->Get_World(&matPlayerWorld);
	//부모 스케일 소거
	_vec3 vPlayerScale, vPlayerTrans;
	D3DXQUATERNION rotQ;
	D3DXMatrixDecompose(&vPlayerScale, &rotQ, &vPlayerTrans, &matPlayerWorld);
	_matrix R; D3DXMatrixRotationQuaternion(&R, &rotQ);
	_matrix T; D3DXMatrixTranslation(&T, vPlayerTrans.x, vPlayerTrans.y, vPlayerTrans.z);
	_matrix matPlayerWorld_DeleteScale = R * T;
	//손 월드 행렬
	D3DXMatrixIdentity(&m_matWorldHand);
	m_matWorldHand = m_matLocalHand * matRev * matPlayerWorld_DeleteScale;

	return S_OK;
}

void CPlayerHand::LateUpdate_GameObject(const _float& fTimeDelta)
{

	
	
}

void CPlayerHand::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorldHand);

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT	CPlayerHand::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CCubeTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_CubeTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_PlayerHandTexture"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	//pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	//if (nullptr == pComponent)
	//	return E_FAIL;
	//m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


	return S_OK;
}

void CPlayerHand::Set_LocalMatrix(HAND_ID newHand)
{
	m_eHand = newHand;

	_matrix matScale, matTrans;

	D3DXMatrixScaling(&matScale, 0.2f, 0.3f, 0.3f);


	switch (m_eHand) {
	case HAND_LEFT:
		D3DXMatrixTranslation(&matTrans, 1.2f, 0.f, 0.f);
		break;
	case HAND_RIGHT:
		D3DXMatrixTranslation(&matTrans, -1.2f, 0.f, 0.f);
		break;
	}
	
	D3DXMatrixIdentity(&m_matLocalHand);
	m_matLocalHand = matScale * matTrans;
}

CPlayerHand* CPlayerHand::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerHand* pHand = new CPlayerHand(pGraphicDev);
	if (FAILED(pHand->Ready_GameObject())) {
		Safe_Release(pHand);
		MSG_BOX("Hand Create Failed");
		return nullptr;
	}
	return pHand;
}

void CPlayerHand::Free()
{

}
