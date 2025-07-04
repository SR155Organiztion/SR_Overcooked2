#include "pch.h"
#include "CRealPlayer.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CTransform.h"
#include "CLayer.h"
#include "CFontMgr.h"

#include "CPlayerHand.h"

#include "IPlace.h"
#include "IChop.h"

CRealPlayer::CRealPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_ePlayerNum(PLAYERNUM_END), m_bKeyCheck{}, m_bAct{}
	, m_pCursorCarriable(nullptr), m_pCursorStation(nullptr), m_pGrabObj(nullptr)
{
}

CRealPlayer::CRealPlayer(const CGameObject& rhs)
	: Engine::CGameObject(rhs)
	, m_ePlayerNum(PLAYERNUM_END), m_bKeyCheck{}
	, m_pCursorCarriable(nullptr), m_pCursorStation(nullptr), m_pGrabObj(nullptr)
{
}

CRealPlayer::~CRealPlayer()
{
}

HRESULT CRealPlayer::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CCubeTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_CubeTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_PlayerTexture"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pFSMCom = dynamic_cast<Engine::CFSMComponent*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_FSM"));
	if (nullptr == pComponent) return E_FAIL;
	m_pFSMCom->Set_Owner(this); // FSM�� �����ڰ� ������ �Ѱ���
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_FSM", pComponent }); //ȣ�� ������ ���� �����ϱ� ����

	return S_OK;
}

HRESULT CRealPlayer::Ready_Hands()
{
	CPlayerHand* pLeftHand = CPlayerHand::Create(m_pGraphicDev);
	if (nullptr == pLeftHand) return E_FAIL;
	pLeftHand->Set_PlayerComponent(m_pTransformCom, m_pFSMCom); pLeftHand->Init_Hand(HAND_LEFT);
	m_vecHands.push_back(pLeftHand);
	CPlayerHand* pRightHand = CPlayerHand::Create(m_pGraphicDev);
	if (nullptr == pRightHand) return E_FAIL;
	pRightHand->Set_PlayerComponent(m_pTransformCom, m_pFSMCom); pRightHand->Init_Hand(HAND_RIGHT);
	m_vecHands.push_back(pRightHand);

	return S_OK;
}

HRESULT CRealPlayer::Ready_GameObject()
{
	if (FAILED(Add_Component())) return E_FAIL;
	if (FAILED(Ready_Hands())) return E_FAIL;

	m_pFSMCom->Add_State("Player_Idle", new CPlayerIdle);
	m_pFSMCom->Add_State("Player_Move", new CPlayerMove);
	m_pFSMCom->Add_State("Player_Act", new CPlayerAct);
	m_pFSMCom->Change_State("Player_Idle");

	m_pTransformCom->m_vScale = { 1.f, 2.f, 1.f };
	m_pTransformCom->Set_Pos(8.f, 1.f, 5.f);

	//m_stOpt.bApplyGravity = false;

	return S_OK;
}

_int CRealPlayer::Update_GameObject(const _float& fTimeDelta)
{
	//�� �����Ӹ��� Ŀ�� �ʱ�ȭ
	m_pCursorCarriable = nullptr;
	m_pCursorStation = nullptr;

	if (m_bAct[ACT_CHOP]) {

	}

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	for (auto& pHand : m_vecHands) {
		pHand->Update_GameObject(fTimeDelta);
	}
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	if (nullptr == m_pGrabObj) {
		m_pCursorCarriable = Find_Cursor_Carriable(*CInteractMgr::GetInstance()->Get_List(CInteractMgr::CARRY));
		// ���� �� �ִ� Ŀ�� ��¦�Ÿ��� �ϴ� �Լ� �߰��� �ڸ�
	}
	m_pCursorStation = Find_Cursor_Station(*CInteractMgr::GetInstance()->Get_List(CInteractMgr::STATION));
	if (m_pCursorStation) {}// �����̼� Ŀ�� ��¦�Ÿ����ϴ°� �߰��� �ڸ�
	if (m_pGrabObj) Set_GrabObjMat();

	KeyInput();

	return S_OK;
}

void CRealPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CRealPlayer::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	for (auto& pHand : m_vecHands) {
		pHand->Render_GameObject();
	}
	if (Test_Carriable) {
		_vec2 carr{ 500.f,100.f };
		CFontMgr::GetInstance()->Render_Font(L"Font_Default", L"Carriable Cursor off", &carr, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	}
	if (Test_Station) {
		_vec2 sta{ 500.f,150.f };
		CFontMgr::GetInstance()->Render_Font(L"Font_Default", L"Station Cursor off", &sta, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	}


}

CRealPlayer* CRealPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRealPlayer* pPlayer = new CRealPlayer(pGraphicDev);

	if (FAILED(pPlayer->Ready_GameObject())) {
		Safe_Release(pPlayer);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}
	return pPlayer;
}

CGameObject* CRealPlayer::Find_Cursor_Carriable(list<CGameObject*> listCarry)
{
	if (Test_Carriable) return nullptr;
	//���� ã�� �˰��� ���� ����
	list<CGameObject*>::iterator iter = listCarry.begin();

	++iter; //�丶�� ����
	if (iter == listCarry.end()) return nullptr;

	return *iter;
}

CGameObject* CRealPlayer::Find_Cursor_Station(list<CGameObject*> listStation)
{
	if (Test_Station) return nullptr;

	//���� ã�� �˰��� ���� ����
	int i(0);
	for (auto& pStation : listStation)
	{
		if (i == 0) return pStation; //chop
		if (pStation) ++i;

	}

	return nullptr;
}

void CRealPlayer::Set_GrabObjMat()
{
	CTransform* pGrabObj_TransCom = dynamic_cast<CTransform*>(m_pGrabObj->Get_Component(ID_DYNAMIC, L"Com_Transform"));

	_matrix matPlayerWorld, matObj;
	m_pTransformCom->Get_World(&matPlayerWorld);
	_vec3 vecPlayerLook, vecPlayerPos, vecObjPos;
	m_pTransformCom->Get_Info(INFO_POS, &vecPlayerPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vecPlayerLook);
	D3DXVec3Normalize(&vecPlayerLook, &vecPlayerLook);
	vecObjPos = vecPlayerPos + vecPlayerLook * 1;
	pGrabObj_TransCom->Set_Pos(vecObjPos.x, vecObjPos.y, vecObjPos.z);
}

void CRealPlayer::Set_HandGrab_Off()
{
	// ���� State ��ȯ
	dynamic_cast<CFSMComponent*>(m_vecHands[0]->Get_Component(ID_DYNAMIC, L"Com_FSM"))->Change_State("LeftHand_Idle");
	dynamic_cast<CFSMComponent*>(m_vecHands[1]->Get_Component(ID_DYNAMIC, L"Com_FSM"))->Change_State("RighttHand_Idle");
	dynamic_cast<CInteract*>(m_pGrabObj)->Set_Ground(true); // ��� �ִ� ��ü �߷� �ѱ�
	m_pGrabObj = nullptr;
}

void CRealPlayer::Change_HandState(std::string newState)
{
	std::string LeftState = "LeftHand_" + newState;
	std::string RightState = "RightHand_" + newState;
	dynamic_cast<CFSMComponent*>(m_vecHands[0]->Get_Component(ID_DYNAMIC, L"Com_FSM"))->Change_State(LeftState);
	dynamic_cast<CFSMComponent*>(m_vecHands[1]->Get_Component(ID_DYNAMIC, L"Com_FSM"))->Change_State(RightState);
}

void CRealPlayer::KeyInput()
{
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_SPACE) & 0x80) {// Ű�Է�
		if (m_bKeyCheck[DIK_SPACE])
			return;
		m_bKeyCheck[DIK_SPACE] = true;
		if (m_pGrabObj) { // ����ִ��� Ȯ��
			if (m_pCursorStation) { //��� ���� ��, stationĿ�� �ִ��� Ȯ��
				// m_pCursorStation�� �ִٸ�, ����ִ� ��ü �������� -> ���� ������ ����
				if (dynamic_cast<IPlace*>(m_pCursorStation)->Set_Place(m_pGrabObj, m_pCursorStation)) m_pGrabObj = nullptr;
			}
			else {// m_pCursorStation�� ���ٸ�
				Set_HandGrab_Off(); // �տ��� ���� ��ü�� �߷� On
			}
		}
		else { // ����ִ°� ���ٸ�
			// ��ó�� ���� �� �ִ� �繰 Ž�� �� Ŀ��(m_pCursorCarriable) Ž��
			if (m_pCursorCarriable) { // m_pCursorCarriableĿ���� ���� �ٸ�, 
				m_pGrabObj = m_pCursorCarriable; // Ŀ���� ��� ��ü�� 
				m_pCursorCarriable = nullptr; // Ŀ�� �����
				dynamic_cast<CInteract*>(m_pGrabObj)->Set_Ground(false); // ��� �ִ� ��ü �߷� ����
				// ���� State ��ȯ
				dynamic_cast<CFSMComponent*>(m_vecHands[0]->Get_Component(ID_DYNAMIC, L"Com_FSM"))->Change_State("LeftHand_GrabIdle");
				dynamic_cast<CFSMComponent*>(m_vecHands[1]->Get_Component(ID_DYNAMIC, L"Com_FSM"))->Change_State("RighttHand_GrabIdle");
			}
			else { // ������ Ŀ���� ���ٸ�
				if (m_pCursorStation) { //�ٵ� �����̼� Ŀ���� �ִٸ�?
					m_pGrabObj = dynamic_cast<IPlace*>(m_pCursorStation)->Get_PlacedItem(); // �����̼ǿ� ������Ʈ�� �ִٸ� ��������
				}
			}
		}
	}
	else m_bKeyCheck[DIK_SPACE] = false;

	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_LCONTROL) & 0x80) {// Ű�Է�
		if (m_bKeyCheck[DIK_LCONTROL])
			return;
		m_bKeyCheck[DIK_LCONTROL] = true;

		if (m_pCursorStation) {
			//if (dynamic_cast<IProcess*>(m_pCursorStation)->Enter_Process()) {// �����̼ǿ� ������Ʈ�� �ִٸ�
			//	Change_HandState("Chop");
			//	m_bAct[ACT_CHOP] = true;
			//}
		}







	}
	else m_bKeyCheck[DIK_LCONTROL] = false;


	//---------------------- �׽�Ʈ�� ----------------------//

	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_NUMPAD9) & 0x80)
	{
		if (m_bKeyCheck[DIK_NUMPAD9])
			return;

		m_bKeyCheck[DIK_NUMPAD9] = true;
		Change_HandState("Chop");

	}
	else
	{
		m_bKeyCheck[DIK_NUMPAD9] = false;
	}

	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_N) & 0x80)
	{
		if (m_bKeyCheck[DIK_N])
			return;

		m_bKeyCheck[DIK_N] = true;

		if (Test_Carriable)
			Test_Carriable = false;

		else
			Test_Carriable = true;
	}
	else
	{
		m_bKeyCheck[DIK_N] = false;
	}
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_M) & 0x80)
	{
		if (m_bKeyCheck[DIK_M])
			return;

		m_bKeyCheck[DIK_M] = true;

		if (Test_Station)
			Test_Station = false;

		else
			Test_Station = true;
	}
	else
	{
		m_bKeyCheck[DIK_M] = false;
	}


}

void CRealPlayer::Free()
{
}
