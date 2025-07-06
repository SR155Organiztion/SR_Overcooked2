#include "pch.h"
#include "CPlayerHand.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CHandState.h"

#include "CDInputMgr.h" //테스트용

CPlayerHand::CPlayerHand(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_eHand(HAND_END), m_pOwner(nullptr), m_bVirtualPivot(false)
{
	ZeroMemory(&m_tRevInfo, sizeof(REVINFO));


}

CPlayerHand::CPlayerHand(const CGameObject& rhs)
	: Engine::CGameObject(rhs)
	, m_eHand(HAND_END), m_pOwner(nullptr), m_bVirtualPivot(false)
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

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	

	if (m_bVirtualPivot) {
		Update_VirtualPivot();
	}
	else {
		Set_HandWorldMat();
	}
	
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

void CPlayerHand::Update_VirtualPivot()
{
	// 설거지 모션을 위한 새로운 로컬행렬 
	_matrix matNewScale; D3DXMatrixScaling(&matNewScale, 0.2f, 0.3f, 0.3f);
	_matrix matNewTrans; D3DXMatrixTranslation(&matNewTrans, 0.f, 0.f, 0.f);
	_matrix matNewLocal = matNewScale;// *matNewTrans;

	
	// ② 궤도 회전(Y축) + 추가 반지름 이동
	_matrix TtoCenter; D3DXMatrixTranslation(&TtoCenter, 0.f, 0.f, -1.f);
	_matrix TtoBack; D3DXMatrixTranslation(&TtoBack, 0.f, 0.f, 1.f);
	_matrix OrbitRot; D3DXMatrixRotationY(&OrbitRot, m_tRevInfo->m_fRevAngleY);
	_matrix Tradius; D3DXMatrixTranslation(&Tradius, 0.0f, 0.f, 0.1f);
	_matrix Rtilt; D3DXMatrixRotationZ(&Rtilt, D3DXToRadian(-30.f));

	_matrix Orbit = TtoCenter * Tradius * OrbitRot * Rtilt * TtoBack;
	//부모 스케일 소거
	_matrix PlayerWorld;
	m_pPlayerTransformCom->Get_World(&PlayerWorld);
	_vec3 vPlayerScale, vPlayerTrans;
	D3DXQUATERNION rotQ;
	D3DXMatrixDecompose(&vPlayerScale, &rotQ, &vPlayerTrans, &PlayerWorld);
	printf("parent scale = %.2f, %.2f, %.2f\n", vPlayerScale.x, vPlayerScale.y, vPlayerScale.z);
	_matrix R; D3DXMatrixRotationQuaternion(&R, &rotQ);
	_matrix T; D3DXMatrixTranslation(&T, vPlayerTrans.x, vPlayerTrans.y, vPlayerTrans.z);
	_matrix matPlayerWorld_DeleteScale = R * T;
	m_matWorldHand = matNewLocal * Orbit * matPlayerWorld_DeleteScale;

	
	D3DXVECTOR3 localZero(0.f, 0.f, 0.f);      // ← l-value 변수
	D3DXVECTOR3 handW;
	D3DXVec3TransformCoord(&handW, &localZero, &m_matWorldHand);

	// 2) 피벗(0,0,1) → 월드
	D3DXVECTOR3 localPivot(0.f, 0.f, 1.f);     // ← l-value 변수
	D3DXVECTOR3 pivotW;
	D3DXVec3TransformCoord(&pivotW, &localPivot, &matPlayerWorld_DeleteScale);

		D3DXVECTOR3 diff = handW - pivotW;
		   // 반지름(m)
	

	// 사용 예
	float r = D3DXVec3Length(&diff);
	printf("radius = %.4f m\n", r);
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

	pComponent = m_pFSMCom = dynamic_cast<Engine::CFSMComponent*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_FSM"));
	if (nullptr == pComponent) return E_FAIL;
	m_pFSMCom->Set_Owner(this); // FSM에 소유자가 누군지 넘겨줌
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_FSM", pComponent }); //호출 시점을 직접 관리하기 위함

	return S_OK;
}

void CPlayerHand::Set_HandWorldMat()
{
	//공전행렬
	_matrix matRevTrans; D3DXMatrixTranslation(&matRevTrans, 
		m_tRevInfo->m_vecRevTrans.x, m_tRevInfo->m_vecRevTrans.y, m_tRevInfo->m_vecRevTrans.z);
	_matrix matRevRotX; D3DXMatrixRotationX(&matRevRotX, m_tRevInfo->m_fRevAngleX);
	_matrix matRev = matRevTrans * matRevRotX ;
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
	if (m_bVirtualPivot) {
		m_matWorldHand = m_matVirtualPivot * matRev * matPlayerWorld_DeleteScale;
	}
	else {
		m_matWorldHand = m_matLocalHand * matRev * matPlayerWorld_DeleteScale;
	}
}

void CPlayerHand::Init_Hand(HAND_ID newHand)
{
	m_eHand = newHand;
	if (HAND_LEFT != m_eHand && HAND_RIGHT != m_eHand) { MSG_BOX("Failed to select Hand ID"); return; }

	// Hand의 Local Matri 생성 과정
	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, 0.2f, 0.3f, 0.3f);
	switch (m_eHand) {
	case HAND_LEFT:
		D3DXMatrixTranslation(&matTrans, -0.7f, 0.f, 0.f);
		break;
	case HAND_RIGHT:
		D3DXMatrixTranslation(&matTrans, 0.7f, 0.f, 0.f);
		break;
	}
	
	D3DXMatrixIdentity(&m_matLocalHand);
	m_matLocalHand = matScale * matTrans;


	if (!m_pFSMCom) MSG_BOX("FSM포인터없음");
	// Hand의 State 생성과정
	switch (m_eHand) {
	case HAND_LEFT:
		m_pFSMCom->Add_State("LeftHand_Idle", new CLeftHandIdle);
		m_pFSMCom->Add_State("LeftHand_Grab", new CLeftHandGrab);
		m_pFSMCom->Add_State("LeftHand_Wash", new CLeftHandWash);
		m_pFSMCom->Add_State("LeftHand_Chop", new CLeftHandChop);
		m_pFSMCom->Change_State("LeftHand_Idle");
		break;
	case HAND_RIGHT:
		m_pFSMCom->Add_State("RightHand_Idle", new CRightHandIdle);
		m_pFSMCom->Add_State("RightHand_Grab", new CRightHandGrab);
		m_pFSMCom->Add_State("RightHand_Wash", new CRightHandWash);
		m_pFSMCom->Add_State("RightHand_Chop", new CRightHandChop);
		m_pFSMCom->Change_State("RightHand_Idle");
		break;
	}

	m_tRevInfo = new REVINFO;
	m_tRevInfo->m_vecRevTrans = { 0.f,0.f,0.f };
	m_tRevInfo->m_fRevAngleX = 0.f;
	m_tRevInfo->m_fRevAngleY = 0.f;
	m_tRevInfo->m_fRevAngleZ = 0.f;

}

void CPlayerHand::Set_PlayerComponent(CGameObject* pPlayer, CTransform* pTransCom, CFSMComponent* pFSMCom)
{
	m_pOwner = pPlayer;
	m_pPlayerTransformCom = pTransCom;
	m_pPlayerFMSMCom = pFSMCom;
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
	Safe_Delete(m_tRevInfo);
    Engine::CGameObject::Free();
}
