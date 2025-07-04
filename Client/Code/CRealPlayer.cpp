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
	m_pFSMCom->Set_Owner(this); // FSM에 소유자가 누군지 넘겨줌
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_FSM", pComponent }); //호출 시점을 직접 관리하기 위함

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
	//매 프레임마다 커서 초기화
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
		// 잡을 수 있는 커서 반짝거리게 하는 함수 추가할 자리
	}
	m_pCursorStation = Find_Cursor_Station(*CInteractMgr::GetInstance()->Get_List(CInteractMgr::STATION));
	if (m_pCursorStation) {}// 스테이션 커서 반짝거리게하는거 추가할 자리
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
	//아직 찾는 알고리즘 구현 안함
	list<CGameObject*>::iterator iter = listCarry.begin();

	++iter; //토마토 리턴
	if (iter == listCarry.end()) return nullptr;

	return *iter;
}

CGameObject* CRealPlayer::Find_Cursor_Station(list<CGameObject*> listStation)
{
	if (Test_Station) return nullptr;

	//아직 찾는 알고리즘 구현 안함
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
	// 손의 State 변환
	dynamic_cast<CFSMComponent*>(m_vecHands[0]->Get_Component(ID_DYNAMIC, L"Com_FSM"))->Change_State("LeftHand_Idle");
	dynamic_cast<CFSMComponent*>(m_vecHands[1]->Get_Component(ID_DYNAMIC, L"Com_FSM"))->Change_State("RighttHand_Idle");
	dynamic_cast<CInteract*>(m_pGrabObj)->Set_Ground(true); // 잡고 있는 물체 중력 켜기
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
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_SPACE) & 0x80) {// 키입력
		if (m_bKeyCheck[DIK_SPACE])
			return;
		m_bKeyCheck[DIK_SPACE] = true;
		if (m_pGrabObj) { // 잡고있는지 확인
			if (m_pCursorStation) { //잡고 있을 때, station커서 있는지 확인
				// m_pCursorStation이 있다면, 잡고있는 물체 내려놓음 -> 이후 포인터 지움
				if (dynamic_cast<IPlace*>(m_pCursorStation)->Set_Place(m_pGrabObj, m_pCursorStation)) m_pGrabObj = nullptr;
			}
			else {// m_pCursorStation이 없다면
				Set_HandGrab_Off(); // 손에서 놓고 물체에 중력 On
			}
		}
		else { // 잡고있는게 없다면
			// 근처에 잡을 수 있는 사물 탐색 후 커서(m_pCursorCarriable) 탐색
			if (m_pCursorCarriable) { // m_pCursorCarriable커서가 잡힌 다면, 
				m_pGrabObj = m_pCursorCarriable; // 커서를 잡는 물체로 
				m_pCursorCarriable = nullptr; // 커서 지우기
				dynamic_cast<CInteract*>(m_pGrabObj)->Set_Ground(false); // 잡고 있는 물체 중력 끄기
				// 손의 State 변환
				dynamic_cast<CFSMComponent*>(m_vecHands[0]->Get_Component(ID_DYNAMIC, L"Com_FSM"))->Change_State("LeftHand_GrabIdle");
				dynamic_cast<CFSMComponent*>(m_vecHands[1]->Get_Component(ID_DYNAMIC, L"Com_FSM"))->Change_State("RighttHand_GrabIdle");
			}
			else { // 아이템 커서가 없다면
				if (m_pCursorStation) { //근데 스테이션 커서가 있다면?
					m_pGrabObj = dynamic_cast<IPlace*>(m_pCursorStation)->Get_PlacedItem(); // 스테이션에 오브젝트가 있다면 가져오기
				}
			}
		}
	}
	else m_bKeyCheck[DIK_SPACE] = false;

	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_LCONTROL) & 0x80) {// 키입력
		if (m_bKeyCheck[DIK_LCONTROL])
			return;
		m_bKeyCheck[DIK_LCONTROL] = true;

		if (m_pCursorStation) {
			//if (dynamic_cast<IProcess*>(m_pCursorStation)->Enter_Process()) {// 스테이션에 오브젝트가 있다면
			//	Change_HandState("Chop");
			//	m_bAct[ACT_CHOP] = true;
			//}
		}







	}
	else m_bKeyCheck[DIK_LCONTROL] = false;


	//---------------------- 테스트용 ----------------------//

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
