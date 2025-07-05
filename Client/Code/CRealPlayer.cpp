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
	, m_pCursorCarriable(nullptr), m_pCursorStation(nullptr), m_pGrabObj(nullptr), m_pIChop(nullptr)
{
}

CRealPlayer::CRealPlayer(const CGameObject& rhs)
	: Engine::CGameObject(rhs)
	, m_ePlayerNum(PLAYERNUM_END), m_bKeyCheck{}
	, m_pCursorCarriable(nullptr), m_pCursorStation(nullptr), m_pGrabObj(nullptr), m_pIChop(nullptr)
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
	pLeftHand->Set_PlayerComponent(this, m_pTransformCom, m_pFSMCom); pLeftHand->Init_Hand(HAND_LEFT);
	m_vecHands.push_back(pLeftHand);
	CPlayerHand* pRightHand = CPlayerHand::Create(m_pGraphicDev);
	if (nullptr == pRightHand) return E_FAIL;
	pRightHand->Set_PlayerComponent(this, m_pTransformCom, m_pFSMCom); pRightHand->Init_Hand(HAND_RIGHT);
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
	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = true;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bApplyKnockBack = true;

	return S_OK;
}

_int CRealPlayer::Update_GameObject(const _float& fTimeDelta)
{
	//매 프레임마다 커서 초기화
	m_pCursorCarriable = nullptr;
	m_pCursorStation = nullptr;

	if (m_bAct[ACT_CHOP]) {
		if (1.f <= m_pIChop->Get_Progress()) {
			Escape_Act(ACT_CHOP, false);
		}
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
	//std::wstring str = L"스페이스바 " + std::to_wstring(test[0]) + L" 번 눌림";
	//const _tchar* result = str.c_str();
	//_vec2 vstr{ 100.f, 150.f };
	//CFontMgr::GetInstance()->Render_Font(L"Font_Default", result, &vstr, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	//std::wstring Car = L"Carriable " + std::to_wstring(test[1]) + L" 번 눌림";
	//const _tchar* Carresult = Car.c_str();
	//_vec2 vstr1{ 100.f, 200.f };
	//CFontMgr::GetInstance()->Render_Font(L"Font_Default", Carresult, &vstr1, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

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
	Change_HandState("Idle");
	dynamic_cast<CInteract*>(m_pGrabObj)->Set_Ground(false); 
	m_pGrabObj = nullptr;
}

void CRealPlayer::Change_HandState(std::string newState)
{
	std::string LeftState = "LeftHand_" + newState;
	std::string RightState = "RightHand_" + newState;
	dynamic_cast<CFSMComponent*>(m_vecHands[0]->Get_Component(ID_DYNAMIC, L"Com_FSM"))->Change_State(LeftState);
	dynamic_cast<CFSMComponent*>(m_vecHands[1]->Get_Component(ID_DYNAMIC, L"Com_FSM"))->Change_State(RightState);
}

void CRealPlayer::Escape_Act(ACT_ID eID, _bool IsPause, std::string PlayerState)
{
	if (IsPause) {
		switch (eID) {
		case ACT_CHOP:
			if (m_pIChop) {
				if (IsPause) m_pIChop->Pause_Process();
				m_pIChop = nullptr; 
			}
			break;
		case ACT_WASH:
			//if (m_pIWash) m_pIWash = nullptr; 
			break;
		}
	}
	m_bAct[eID] = false;
	Change_HandState("Idle");
	m_pFSMCom->Change_State(PlayerState);
}

void CRealPlayer::Change_PlayerState(std::string PlayerState)
{
	m_pFSMCom->Change_State(PlayerState);
}

void CRealPlayer::KeyInput()
{


	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_SPACE) & 0x80)
	{
		if (m_bKeyCheck[DIK_SPACE]) return;
		m_bKeyCheck[DIK_SPACE] = true;
		//--------------- Body ---------------//
		if (m_pGrabObj) { // 잡고있는지 확인
			if (m_pCursorStation) { //잡고 있을 때, station커서 있는지 확인
				// m_pCursorStation이 있다면, 잡고있는 물체 내려놓음 -> 이후 포인터 지움
				if (dynamic_cast<IPlace*>(m_pCursorStation)->Set_Place(m_pGrabObj, m_pCursorStation)) {
					m_pGrabObj = nullptr;
					Change_HandState("Idle");
				}
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
				dynamic_cast<CInteract*>(m_pGrabObj)->Set_Ground(true); // 잡고 있는 물체 중력 끄기
				// 손의 State 변환
				Change_HandState("Grab");
				//예누 함수 추가예정 (재료의 넉백, 롤링 꺼줄 함수)
			}
			else { // 아이템 커서가 없다면
				if (m_pCursorStation) { //근데 스테이션 커서가 있다면?
					m_pGrabObj = dynamic_cast<IPlace*>(m_pCursorStation)->Get_PlacedItem(); // 스테이션에 오브젝트가 있다면 가져오기
					if (m_pGrabObj)  Change_HandState("Grab");				//예누 함수 추가예정 (재료의 넉백, 롤링 꺼줄 함수)
				}
			}
		}
	}
	else m_bKeyCheck[DIK_SPACE] = false;


	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_LCONTROL) & 0x80)
	{
		if (m_bKeyCheck[DIK_LCONTROL]) return;
		m_bKeyCheck[DIK_LCONTROL] = true;
		//--------------- Body ---------------//
		if (m_pGrabObj) return;//Set_HandGrab_Off();
		if (dynamic_cast<IChop*>(m_pCursorStation)) {
			m_pIChop = dynamic_cast<IChop*>(m_pCursorStation);
			if (m_pIChop->Enter_Process()) {
				Change_HandState("Chop");
				m_pFSMCom->Change_State("Player_Act");
				m_bAct[ACT_CHOP] = true;
			}
		}
	}
	else m_bKeyCheck[DIK_LCONTROL] = false;
		
		


	
	//스테이션과 상호작용
	//스테이션을 IProcess에서 IChop과 IWash(아직구현x) 다이나믹 캐스트해서(최대 2번) 성공하면 그 안에 함수 사용
	// if (Enter_Process) true반환하면 애니메이션시작
	// 업데이트타임에서 매 프레임마다 Get_progress 받아서
	// 진행도 파악(애니메이션 탈출용)
	// 만약 중간에 이탈 시, pause_process 호출.
	// get_progress == 1일때 애니메이션 끝.
	
	//Change_HandState("")



	//if (pInput->Get_DIKeyState(DIK_P)) {
	//	dynamic_cast<CFSMComponent*>(pPlayer->Get_Component(ID_DYNAMIC, L"Com_FSM"))->Change_State("Player_Act");
	//	for (_int i = HAND_LEFT; i < HAND_END; ++i) {
	//		HAND_ID eID = static_cast<HAND_ID>(i);
	//		switch (eID) {
	//		case HAND_LEFT:
	//			pPlayer->Get_Hand(eID)->Get_HandFSMCom()->Change_State("LeftHand_Chop");
	//			break;
	//		case HAND_RIGHT:
	//			pPlayer->Get_Hand(eID)->Get_HandFSMCom()->Change_State("RightHand_Chop");
	//			break;
	//		default:
	//			MSG_BOX("HandState Change Fail / State_Idle");
	//		}
	//	}
	//}

	KEY_ONCE(DIK_LCONTROL, {
		if (m_pCursorStation) {
			//if (dynamic_cast<IProcess*>(m_pCursorStation)->Enter_Process()) {// 스테이션에 오브젝트가 있다면
			//	Change_HandState("Chop");
			//	m_bAct[ACT_CHOP] = true;
			//}
		}
	});
	//---------------------- 테스트용 ----------------------//

	KEY_ONCE(DIK_N, {
		++test[1];
		if (Test_Carriable)
			Test_Carriable = false;

		else
			Test_Carriable = true;
	});
	KEY_ONCE(DIK_M, {
		++test[2];
		if (Test_Station)
			Test_Station = false;

		else
			Test_Station = true;
	});
}

void CRealPlayer::Free()
{
	for (auto pHand : m_vecHands) { Safe_Release(pHand); }
	Engine::CGameObject::Free();
}
