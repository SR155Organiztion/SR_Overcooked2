#include "pch.h"
#include "CRealPlayer.h"
#include "CPlayerHand.h"
#include "CTransform.h"

#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CLayer.h"
#include "CFontMgr.h"
#include "CDInputMgr.h"
#include "CManagement.h"
#include "CEffectMgr.h"
#include "CSoundMgr.h"
#include "CUtil.h"

#include "IPlace.h"
#include "IChop.h"
#include "CGasStation.h"
#include "CChopStation.h"
#include "CIngredientStation.h"
#include "CFireExtinguisher.h"
#include "CInvisibleStation.h"

#include "COnionKing.h"


CRealPlayer::CRealPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_ePlayerNum(PLAYERNUM_END), m_bKeyCheck{}, m_bAct{}
	, m_pCursorCarriable(nullptr), m_pCursorStation(nullptr), m_pGrabObj(nullptr), m_pIChop(nullptr), m_pActStation(nullptr), m_strCurName{}
	, test{}, m_szShowTestTime{}, m_bTestAct{}
{
}

CRealPlayer::CRealPlayer(const CGameObject& rhs)
	: Engine::CGameObject(rhs)
	, m_ePlayerNum(PLAYERNUM_END), m_bKeyCheck{}
	, m_pCursorCarriable(nullptr), m_pCursorStation(nullptr), m_pGrabObj(nullptr), m_pIChop(nullptr), m_pActStation(nullptr), m_strCurName{}
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

	m_pTransformCom->m_vScale = { 0.8f, 1.6f, 0.8f };
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f));

	m_ePlayerNum = PLAYER_1P; 

	//m_stOpt.bApplyGravity = false;
	m_stOpt.bApplyGravity = true;
	//m_stOpt.bApplyRolling = true;
	//m_stOpt.bApplyBouncing = false;
	//m_stOpt.bApplyKnockBack = true;
	m_stOpt.bPushable = true;
	m_stOpt.bEnableLookCol = true;


	
	return S_OK;
}

_int CRealPlayer::Update_GameObject(const _float& fTimeDelta)
{
	Check_TestCool(fTimeDelta); //이펙트 테스트용. 삭제예정
	m_bAct[ACT_EXTINGUISH] = false;

	Reset_Cursor();
	Check_Act(fTimeDelta);
	Check_NotSnap(fTimeDelta);
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	Update_Hands(fTimeDelta);
	Set_Cursor();
	Set_GrabObjMat();
	KeyInput();
	Check_CursorName();
	Reset_DetectedList();

	if (dynamic_cast<CFireExtinguisher*>(m_pGrabObj) && m_bPreAct[ACT_EXTINGUISH] && !m_bAct[ACT_EXTINGUISH]) {
 		dynamic_cast<CFireExtinguisher*>(m_pGrabObj)->Pause_Process();
	}
	m_bPreAct[ACT_EXTINGUISH] = m_bAct[ACT_EXTINGUISH];

	return S_OK;
}

void CRealPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CRealPlayer::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(static_cast<_uint>(m_ePlayerNum));

	Engine::CGameObject::Set_Material();

	m_pBufferCom->Render_Buffer();

	for (auto& pHand : m_vecHands) {
		pHand->Render_GameObject();
	}

	Render_TestName();

}

void CRealPlayer::Check_Act(const _float& dt)
{
	if (m_bAct[ACT_CHOP]) {
		if (1.f <= m_pIChop->Get_Progress()) {
			Escape_Act(ACT_CHOP, false);
		}
	}
	if (m_bAct[ACT_WASH]) {
		if (1.f <= m_pIWash->Get_Progress()) { 
			Escape_Act(ACT_WASH, false);
		}
	
	}
	if (m_bTestAct[ACT_CHOP]) {
		test[0] += dt;
		swprintf_s(m_szShowTestTime, L"Chop %f", test[0]);	// 테스트
		if (5 <= test[0]) {
			Escape_Act(ACT_CHOP, false);
			m_bTestAct[ACT_CHOP] = false;
		}
	}
	if (m_bTestAct[ACT_WASH]) {
		test[1] += dt;
		swprintf_s(m_szShowTestTime, L"Wash %f", test[1]);	// 테스트
		if (5 <= test[1]) {
			Escape_Act(ACT_WASH, true);
			m_bTestAct[ACT_WASH] = false;
		}
	}
	if (m_bTestAct[ACT_SURPRISED]) {
		test[1] += dt;
		swprintf_s(m_szShowTestTime, L"Surprised %f", test[3]);	// 테스트
		if (5 <= test[3]) {
			Escape_Act(ACT_SURPRISED, true);
			m_bTestAct[ACT_SURPRISED] = false;
		}
	}
}

void CRealPlayer::Shine_Cursor(CGameObject* pCursor)
{
	if (!pCursor)
		return;
	dynamic_cast<CInteract*>(pCursor)->Set_Highlight(true);
}

void CRealPlayer::GrabKey_Algorithm()
{
	if (m_pGrabObj) { // 손에 잡고있는지 확인
		if (m_pCursorStation) { //잡고 있을 때, station커서 있는지 확인
			IPlace* pStation = dynamic_cast<IPlace*>(m_pCursorStation); // Station 편하게 쓰려고 미리 다운캐스팅
			CInteract* pStationOnItem = dynamic_cast<CInteract*>(pStation->Get_Item()); // Station에 아이템있는지 미리 받음. 예외처리 꼭 하고 쓸것

			CInteract::INTERACTTYPE eGrab = dynamic_cast<CInteract*>(m_pGrabObj)->Get_InteractType(); // 손에 잡힌게 뭔지
			switch (eGrab) {
			case CInteract::INGREDIENT: // 손에 잡힌게 재료
				if (pStationOnItem) { // Station에 물건이 있다면
					if (CInteract::EXTINGUISHER == pStationOnItem->Get_InteractType())  // Station위에 소화기가 있다면
						return;
					else if (CInteract::INGREDIENT != pStationOnItem->Get_InteractType()) { // Station위에 도구가 있다면
						if (dynamic_cast<IPlace*>(pStation->Get_Item())->Set_Place(m_pGrabObj, pStation->Get_Item())) { // Station위에 도구가 있다면 손에 들고있는 재료를 도구에 넣는 시도
							m_pGrabObj = nullptr;
							Change_HandState("Idle");
						}
					}
				}
				else { //Station에 도구가 없다면
					if (pStation->Set_Place(m_pGrabObj, m_pCursorStation)) { //Station위에 도구 없다면 손에 들고있는 재료를 station위에 올리는 시도
						m_pGrabObj = nullptr;
						Change_HandState("Idle");
					}
				}
				break;
			case CInteract::FRYINGPAN:	//@@@@@@@@@@@@@   Station 위 도구와 손에 든 도구 상호작용 !금지! 구현안함
			case CInteract::POT:		// 손에 잡힌게 도구고
				if (pStationOnItem) {	// Station에 아이템이 있다면
					if (CInteract::EXTINGUISHER == pStationOnItem->Get_InteractType())  // Station위에 소화기가 있다면
						return;
					else if (CInteract::INGREDIENT == dynamic_cast<CInteract*>(pStationOnItem)->Get_InteractType()) { // Station위에 재료라면
						dynamic_cast<IPlace*>(m_pGrabObj)->Set_Place(pStation->Get_PlacedItem(), m_pGrabObj);//Station위 재료를 손에 든 식기류에 넣는 시도
					}
					else if (CInteract::PLATE == dynamic_cast<CInteract*>(pStationOnItem)->Get_InteractType()) { // Station위에 접시라면
						dynamic_cast<IPlace*>(pStation->Get_Item())->Set_Place(m_pGrabObj, pStation->Get_Item()); //Station위 접시에 손에 든 식기류위의 재료를 넣는 시도
					}
				}
				else { //Station에 물건이 없다면
					if (pStation->Set_Place(m_pGrabObj, m_pCursorStation)) { //Station위에 물건 없다면 손에 들고있는 도구를 station위에 올리는 시도
						m_pGrabObj = nullptr;
						Change_HandState("Idle");
					}
				}
				break;
			case CInteract::PLATE:
				if (pStationOnItem) { // Station에 물건이 있다면
					if (CInteract::EXTINGUISHER == pStationOnItem->Get_InteractType())  // Station위에 소화기가 있다면
						return;
					else if (CInteract::INGREDIENT == dynamic_cast<CInteract*>(pStation->Get_Item())->Get_InteractType()) { // Station위 물건이 재료라면
						dynamic_cast<IPlace*>(m_pGrabObj)->Set_Place(pStation->Get_PlacedItem(), m_pGrabObj); // station위 재료를 접시에 올리려 시도
					}
					else if (CInteract::INGREDIENT != dynamic_cast<CInteract*>(pStation->Get_Item())->Get_InteractType()) { // Station위 물건이 식기류 or 접시라면
						dynamic_cast<IPlace*>(m_pGrabObj)->Set_Place(dynamic_cast<IPlace*>(pStation->Get_Item())->Get_PlacedItem(), m_pGrabObj); // station위 식기류 or 접시에 있는 재료를 접시에 올리려 시도
					 }
				}
				else { //station에 아무것도 없다면
					if (pStation->Set_Place(m_pGrabObj, m_pCursorStation)) { //Station위에  없다면 손에 들고있는 접시를를 station위에 올리는 시도
						m_pGrabObj = nullptr;
						Change_HandState("Idle");
					}
				}
			case CInteract::EXTINGUISHER:
				if (pStation->Set_Place(m_pGrabObj, m_pCursorStation)) { //Station위에 물건 없다면 손에 들고있는 도구를 station위에 올리는 시도
					m_pGrabObj = nullptr;
					Change_HandState("Idle");
				}
			}
		}
		else {// m_pCursorStation이 없다면
			if (m_pCursorCarriable) {
				CInteract::INTERACTTYPE eID = dynamic_cast<CInteract*>(m_pGrabObj)->Get_InteractType(); // 잡은 물건의 ID 확인
				switch (eID) {
				case CIngredient::INGREDIENT: // 잡고 있는게 재료라면 
				{
					IPlace* pTool = dynamic_cast<IPlace*>(m_pCursorCarriable);
					if (pTool) { // 커서가 도구라면
						if (pTool->Set_Place(m_pGrabObj, m_pCursorCarriable)) { // 손에 재료를 들고 있고 도구가 커서로 잡힌다면 도구에 넣는 시도
							m_pGrabObj = nullptr;
							Change_HandState("Idle");
						}
					}
				}
				break;
				case CIngredient::FRYINGPAN:
				case CIngredient::POT: //잡고 있는게 도구라면
				{
					if (CInteract::INGREDIENT == dynamic_cast<CInteract*>(m_pCursorCarriable)->Get_InteractType()) { // 커서가 재료라면
						dynamic_cast<IPlace*>(m_pGrabObj)->Set_Place(m_pCursorCarriable, m_pGrabObj);//손에든 재료를 손에 든 식기류에 넣는 시도
					}
					else if (CInteract::PLATE == dynamic_cast<CInteract*>(m_pCursorCarriable)->Get_InteractType()) { // 커서가 접시라면
						dynamic_cast<IPlace*>(m_pCursorCarriable)->Set_Place(m_pGrabObj, m_pCursorCarriable); //커서로 잡힌 접시에 손에 든 식기류위의 재료를 넣는 시도
					}
				}
				break;
				case CIngredient::PLATE: // 잡고 있는게 접시라면
				{
					if (m_pCursorCarriable) { // 커서가 잡혀있을 때
						CInteract::INTERACTTYPE CursorID = dynamic_cast<CInteract*>(m_pCursorCarriable)->Get_InteractType(); // 커서가 뭐냐?
						switch (CursorID) {
						case CInteract::INGREDIENT: // 잡고있는게 접시고 커서로 재료가 잡히면
							dynamic_cast<IPlace*>(m_pGrabObj)->Set_Place(m_pCursorCarriable, m_pGrabObj);
							break;
						case CIngredient::FRYINGPAN:
						case CIngredient::POT:
						case CIngredient::PLATE: //잡고 있는게 접시고 커서로 도구가 잡히면
							dynamic_cast<IPlace*>(m_pGrabObj)->Set_Place(m_pCursorCarriable, m_pGrabObj);
						}
					}
					else
						Drop_GrabObject(); // 잡고있는 물체가 상호작용할 커서가 없으면 손에서 놓음
				}
				break;
				}
			}
			else {
				Drop_GrabObject(); // 잡고있는 물체가 상호작용할 커서가 없으면 손에서 
			}
		}
	}
	else { // 손에 잡고있는게 없다면
		// 근처에 잡을 수 있는 사물 탐색 후 커서(m_pCursorCarriable) 탐색
		if (m_pCursorCarriable) { // m_pCursorCarriable커서가 잡힌 다면, 
			m_pGrabObj = m_pCursorCarriable; // 커서를 잡는 물체로 
			CSoundMgr::GetInstance()->Play_Sound(PLAYER_PICKUP, PLAYER_CHANNEL);
			m_pCursorCarriable = nullptr; // 커서 지우기
			dynamic_cast<CInteract*>(m_pGrabObj)->Set_Ground(true); // 잡고 있는 물체 중력 끄기
			// 손의 State 변환
			Change_HandState("Grab");
			//예누 함수 추가예정 (재료의 넉백, 롤링 꺼줄 함수)
		}
		else { // 아이템 커서가 없다면
			if (m_pCursorStation) { //근데 스테이션 커서가 있다면?
				m_pGrabObj = dynamic_cast<IPlace*>(m_pCursorStation)->Get_PlacedItem(); // 스테이션에 오브젝트가 있다면 가져오기
				CIngredientStation* pIngrediStation = dynamic_cast<CIngredientStation*>(m_pCursorStation);
				if (m_pGrabObj) {
					CSoundMgr::GetInstance()->Play_Sound(PLAYER_PICKUP, PLAYER_CHANNEL);
					Change_HandState("Grab");				//예누 함수 추가예정 (재료의 넉백, 롤링 꺼줄 함수)
					dynamic_cast<CInteract*>(m_pGrabObj)->Set_Ground(true); // 잡고 있는 물체 중력 끄기
				}
				else if (!m_pGrabObj && pIngrediStation) {
					CGameObject* pIngre = pIngrediStation->TakeOut_Ingredient();
					if (pIngre) {
						CSoundMgr::GetInstance()->Play_Sound(PLAYER_PICKUP, PLAYER_CHANNEL);
						m_pGrabObj = pIngre;
						Change_HandState("Grab");				//예누 함수 추가예정 (재료의 넉백, 롤링 꺼줄 함수)
						dynamic_cast<CInteract*>(m_pGrabObj)->Set_Ground(true); // 잡고 있는 물체 중력 끄기
					}
				}
			}
		}
	}
}

void CRealPlayer::ActKey_Algorithm()
{
	if (m_pGrabObj) {
		CInteract::INTERACTTYPE eGrab = dynamic_cast<CInteract*>(m_pGrabObj)->Get_InteractType();
		if (eGrab == CInteract::INGREDIENT) {
			CSoundMgr::GetInstance()->Play_Sound(PLAYER_THROW, PLAYER_CHANNEL);
			_vec3 vLook; m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
			D3DXVec3Normalize(&vLook, &vLook);
			CInteract* pInteract = dynamic_cast<CInteract*>(m_pGrabObj);
			pInteract->Be_Thrown(vLook, 10.f);
			pInteract->Set_Ground(false);
			m_pGrabObj = nullptr;
			Change_HandState("Throw");
			m_bNotSnap = true;
			m_fNotSnapCool = 0.f;
		}
	}
	else {
		if (dynamic_cast<IChop*>(m_pCursorStation)) {
			m_pIChop = dynamic_cast<IChop*>(m_pCursorStation);
			if (m_pIChop->Enter_Process()) {
				Change_HandState("Chop");
				m_pFSMCom->Change_State("Player_Act");
				m_pActStation = m_pCursorStation;
				m_bAct[ACT_CHOP] = true;
			}
		}
		else if (dynamic_cast<IWash*>(m_pCursorStation)) {
			m_pIWash = dynamic_cast<IWash*>(m_pCursorStation);
			if (m_pIWash->Enter_Process()) {
				CSoundMgr::GetInstance()->Play_Sound(PLAYER_WASHING, PLAYER_WASH_CHANNEL);
				Change_HandState("Wash");
				m_pFSMCom->Change_State("Player_Act");
				m_pActStation = m_pCursorStation;
				m_bAct[ACT_WASH] = true;
			}
		}
	}
}

void CRealPlayer::ActKey_Extinguish()
{
	CInteract::INTERACTTYPE eGrab = dynamic_cast<CInteract*>(m_pGrabObj)->Get_InteractType();

	if (eGrab == CInteract::EXTINGUISHER) {
		_vec3 vLook; m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		dynamic_cast<CFireExtinguisher*>(m_pGrabObj)->Enter_Process(vLook);
		m_bAct[ACT_EXTINGUISH] = true;
	}
}

void CRealPlayer::Check_NotSnap(const _float& dt)
{
	if (m_bNotSnap) {
		m_fNotSnapCool += dt;
		if (0.7f < m_fNotSnapCool) {
			m_bNotSnap = false;
			m_fNotSnapCool = 0.f;
		}
	}
}

void CRealPlayer::Random_Sound()
{
	vector<SOUND_ID> s_vec;
	s_vec.push_back(SOUND_ID::PLAYER_PICKUP);

	int i = CUtil::Make_Random<int>(0, s_vec.size());

	s_vec[i];
}

void CRealPlayer::Check_CursorName()
{
	if (m_pCursorCarriable) {

		if (CInteract::INGREDIENT == dynamic_cast<CInteract*>(m_pCursorCarriable)->Get_InteractType()) {
			CIngredient::INGREDIENT_TYPE eID = dynamic_cast<CIngredient*>(m_pCursorCarriable)->Get_IngredientType();
			switch (eID) {
			case CIngredient::SEAWEED:
				m_strCurName[CURSOR_INGREDIENT] = L"Seaweed";
				break;
			case CIngredient::LETTUCE:
				m_strCurName[CURSOR_INGREDIENT] = L"Lettuce";
				break;
			case CIngredient::TOMATO:
				m_strCurName[CURSOR_INGREDIENT] = L"Tomato";
				break;
			case CIngredient::CUCUMBER:
				m_strCurName[CURSOR_INGREDIENT] = L"Cucumber";
				break;
			case CIngredient::FISH:
				m_strCurName[CURSOR_INGREDIENT] = L"Fish";
				break;
			case CIngredient::SHRIMP:
				m_strCurName[CURSOR_INGREDIENT] = L"Shrimp";
				break;
			case CIngredient::RICE:
				m_strCurName[CURSOR_INGREDIENT] = L"Rice";
				break;
			case CIngredient::PASTA:
				m_strCurName[CURSOR_INGREDIENT] = L"Pasta";
				break;
			}
		}
		else {
			switch (dynamic_cast<CInteract*>(m_pCursorCarriable)->Get_InteractType()) {
			case CInteract::FRYINGPAN:
				m_strCurName[CURSOR_INGREDIENT] = L"Frypan";
				break;
			case CInteract::POT:
				m_strCurName[CURSOR_INGREDIENT] = L"Pot";
;				break;
			case CInteract::PLATE:
				m_strCurName[CURSOR_INGREDIENT] = L"Plate";
				break;
			}
		}
	}
	else m_strCurName[CURSOR_INGREDIENT] = L"";

	if (m_pCursorStation) {
		CInteract::INTERACTTYPE eID = dynamic_cast<CInteract*>(m_pCursorStation)->Get_InteractType();
		switch (eID) {
		case CInteract::STATION:
			if (dynamic_cast<CGasStation*>(m_pCursorStation)) {
				m_strCurName[CURSOR_STATION] = L"Gas_Station";
			}
			else if (dynamic_cast<CIngredientStation*>(m_pCursorStation)) {
				const _tchar* sz = dynamic_cast<CIngredientStation*>(m_pCursorStation)->Get_IngredientName();
				m_strCurName[CURSOR_STATION] = std::wstring(L"Ingredient_Station ") + sz;
			}
			else {
				m_strCurName[CURSOR_STATION] = L"Undefined_Station";
			}
			break;
		case CInteract::CHOPSTATION:
			m_strCurName[CURSOR_STATION] = L"Chop_Station";
			break;
		case CInteract::SINKSTATION:
			m_strCurName[CURSOR_STATION] = L"Sink_Station";
			break;
		case CInteract::EMPTYSTATION:
			m_strCurName[CURSOR_STATION] = L"Empty_Station";
			break;
		default:
			m_strCurName[CURSOR_STATION] = L"";
		}
		//if (dynamic_cast<IPlace*>(m_pCursorStation)->Is_Full()) {
		//	m_strCurName[CURSOR_STATION_ON_ITEM] = L"On_item";
		//}
		//else m_strCurName[CURSOR_STATION_ON_ITEM] = L"";
		 
	}
	else m_strCurName[CURSOR_STATION] = L"";
	
}

void CRealPlayer::Render_TestName()
{
	if (m_pCursorCarriable) {
		std::wstring ingre = L"Ingredient : " + m_strCurName[CURSOR_INGREDIENT];
		const _tchar* Ingre_result = ingre.c_str();
		_vec2 vIngre{ 100.f, 200.f };
		CFontMgr::GetInstance()->Render_Font(L"Font_Default", Ingre_result, &vIngre, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	}
	if (m_pCursorStation) {
		std::wstring station = L"Station : " + m_strCurName[CURSOR_STATION] + L" " + m_strCurName[CURSOR_STATION_ON_ITEM];
		const _tchar* station_result = station.c_str();
		_vec2 vstation{ 100.f, 250.f };
		CFontMgr::GetInstance()->Render_Font(L"Font_Default", station_result, &vstation, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	}
	if (Test_Carriable) {
		_vec2 carr{ 500.f,100.f };
		CFontMgr::GetInstance()->Render_Font(L"Font_Default", L"Carriable Cursor off", &carr, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	}
	if (Test_Station) {
		_vec2 sta{ 500.f,150.f };
		CFontMgr::GetInstance()->Render_Font(L"Font_Default", L"Station Cursor off", &sta, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	}
	if (m_bTestAct[ACT_CHOP] || m_bTestAct[ACT_WASH]) {
		_vec2 vChoptime{ 500.f, 300.f };
		CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szShowTestTime, &vChoptime, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	}

}

void CRealPlayer::Check_TestCool(const _float& dt)
{
	//테스트용 지울예정
	
	// 스팀 이펙트 확인용
	//if (m_bSteam) {
	//	m_fSteamCool += fTimeDelta;
	//	if (0.1f < m_fSteamCool) {
	//		m_bSteam = false;
	//		m_fSteamCool = 0.f;
	//	}
	//}
	//if (!m_bSteam && m_ePlayerNum == PLAYER_2P) {
	//	CEffectMgr::GetInstance()->Play_Effect(L"SteamEffect", this);
	//	m_bSteam = true;
	//	m_fSteamCool = 0.f;
	//}

	// 소화기 이펙트 확인용
	if (m_bExtinguish) {
		m_fExtinguishCool += dt;
		if (0.1f < m_fExtinguishCool) {
			m_bExtinguish = false;
		}
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

CGameObject* CRealPlayer::Find_Cursor(CURSOR_ID eID)
{
	if (CURSOR_ALL > eID || CURSOR_END < eID) return nullptr;
	
	if (!m_listDetected[eID].empty()) return m_listDetected[eID].front();

	return nullptr;
}

void CRealPlayer::Update_Hands(const _float dt)
{
	for (auto& pHand : m_vecHands) {
		pHand->Update_GameObject(dt);
	}
}

void CRealPlayer::Set_Cursor()
{
	if (nullptr == m_pGrabObj) { //잡은게 없을때, 커서로 아무거나 가리킴
		m_pCursorCarriable = Find_Cursor(CURSOR_ALL);
			Shine_Cursor(m_pCursorCarriable);
	}
	else {
		CInteract* pGrab = dynamic_cast<CInteract*>(m_pGrabObj);
		if (pGrab) {//뭔가 잡고있을때
			CInteract::INTERACTTYPE eID = pGrab->Get_InteractType();
			switch (eID) {
			case CInteract::INGREDIENT: //그게 재료일때 도구만 가리킴
				m_pCursorCarriable = Find_Cursor(CURSOR_TOOL);
				Shine_Cursor(m_pCursorCarriable);
				break;
			case CInteract::FRYINGPAN: //그게 도구들일 때, 재료랑 접시만 가리킴
			case CInteract::POT:
				m_pCursorCarriable = Find_Cursor(CURSOR_NOTOOL);
				Shine_Cursor(m_pCursorCarriable);
				break;
			case CInteract::PLATE: // 잡고있는게 접시일 때, 모두 가리킴
				m_pCursorCarriable = Find_Cursor(CURSOR_ALL);
				if (dynamic_cast<CFireExtinguisher*>(m_pCursorCarriable)) {
					m_pCursorCarriable = nullptr;
					break;
				}
				Shine_Cursor(m_pCursorCarriable);
				break;
			}
		}
	}
	m_pCursorStation = Find_Cursor(CURSOR_STATION);
	if (m_pCursorStation) Shine_Cursor(m_pCursorStation);
}

void CRealPlayer::Set_GrabObjMat()
{
	if (!m_pGrabObj) return;

	CTransform* pGrabObj_TransCom = dynamic_cast<CTransform*>(m_pGrabObj->Get_Component(ID_DYNAMIC, L"Com_Transform"));

	_matrix matPlayerWorld, matObj;
	m_pTransformCom->Get_World(&matPlayerWorld);
	_vec3 vecPlayerLook, vecPlayerPos, vecObjPos;
	m_pTransformCom->Get_Info(INFO_POS, &vecPlayerPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vecPlayerLook);
	D3DXVec3Normalize(&vecPlayerLook, &vecPlayerLook);
	vecObjPos = vecPlayerPos + vecPlayerLook * 1;
	pGrabObj_TransCom->Set_Pos(vecObjPos.x, vecObjPos.y + 0.4f, vecObjPos.z);
}

void CRealPlayer::Drop_GrabObject()
{
	CSoundMgr::GetInstance()->Play_Sound(PLAYER_PUTDOWN, PLAYER_CHANNEL);
	Change_HandState("Idle");
	dynamic_cast<CInteract*>(m_pGrabObj)->Set_Ground(false); 
	m_pGrabObj = nullptr;
	m_bNotSnap = true;
	m_fNotSnapCool = 0.f;
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
	switch (eID) {
	case ACT_CHOP:
		if (m_pIChop) {
			if (IsPause) m_pIChop->Pause_Process();
			m_pActStation = nullptr;
			m_pIChop = nullptr; 
		}
		break;
	case ACT_WASH:
		if (m_pIWash) {
			if (IsPause) 
				m_pIWash->Pause_Process();
			CSoundMgr::GetInstance()->Stop_Sound(PLAYER_WASH_CHANNEL);
			m_pActStation = nullptr;
			m_pIWash = nullptr;
		}

		break;
	}
	m_bTestAct[eID] = false; //테스트
	m_bAct[eID] = false;
	Change_HandState("Idle");

	m_pFSMCom->Change_State(PlayerState);
}

void CRealPlayer::Change_PlayerState(std::string PlayerState)
{
	m_pFSMCom->Change_State(PlayerState);
}

CGameObject* CRealPlayer::Get_CursorStation()
{
	if (!m_pActStation)
		return nullptr;

	return m_pActStation;
}

void CRealPlayer::On_Detected(CGameObject* _pGameObject)
{
	CInteract* pInteract = dynamic_cast<CInteract*>(_pGameObject);
	if (nullptr == pInteract) return;

	switch (pInteract->Get_InteractType()) {
	case CInteract::INGREDIENT:
		if (pInteract->Get_Ground()) break;
		m_listDetected[CURSOR_ALL].push_back(pInteract);
		m_listDetected[CURSOR_NOTOOL].push_back(pInteract);
		m_listDetected[CURSOR_INGREDIENT].push_back(pInteract);
		break;
	case CInteract::FRYINGPAN:
	case CInteract::POT:		
		m_listDetected[CURSOR_ALL].push_back(pInteract);
		m_listDetected[CURSOR_TOOL].push_back(pInteract);
		break; 
	case CInteract::PLATE:
		m_listDetected[CURSOR_ALL].push_back(pInteract);
		m_listDetected[CURSOR_TOOL].push_back(pInteract);
		m_listDetected[CURSOR_NOTOOL].push_back(pInteract);
		break;
	case CInteract::EXTINGUISHER:
		m_listDetected[CURSOR_ALL].push_back(pInteract);
	}
}

void CRealPlayer::On_Collision(CGameObject* _pGameObject)
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(_pGameObject);
	if (!pIngredient || m_bNotSnap)
		return;

	if (pIngredient->Get_Opt()->bThrown) {
		if ("Player_Act" != m_pFSMCom->GerCurrStateName()) {
			if (m_pGrabObj) {
				Drop_GrabObject();
				CSoundMgr::GetInstance()->Play_Sound(PLAYER_CATCH, PLAYER_CHANNEL);
				m_pGrabObj = _pGameObject;
				dynamic_cast<CInteract*>(m_pGrabObj)->Set_Ground(true); // 잡고 있는 물체 중력 끄기
				Change_HandState("Grab");
				pIngredient->Get_Opt()->bThrown = false;
				dynamic_cast<CTransform*>(pIngredient->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Velocity({ 0.f, 0.f, 0.f }, 0.f);
			}
			else {
				m_pGrabObj = _pGameObject;
				if (m_pGrabObj) {
					CSoundMgr::GetInstance()->Play_Sound(PLAYER_CATCH, PLAYER_CHANNEL);
					dynamic_cast<CInteract*>(m_pGrabObj)->Set_Ground(true); // 잡고 있는 물체 중력 끄기
					Change_HandState("Grab");
					pIngredient->Get_Opt()->bThrown = false;
					dynamic_cast<CTransform*>(pIngredient->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Velocity({ 0.f, 0.f, 0.f }, 0.f);

				}
			}
		}
	}

}

void CRealPlayer::On_LookHit(CGameObject* _pGameObject) {
	CInteract* pInteract = dynamic_cast<CInteract*>(_pGameObject);
	if (nullptr == pInteract) return;

	switch (pInteract->Get_InteractType()) {
	case CInteract::STATION:
		if (dynamic_cast<CInvisibleStation*>(_pGameObject))
			break;
	case CInteract::CHOPSTATION:
	case CInteract::SINKSTATION:
	case CInteract::EMPTYSTATION:
		m_listDetected[CURSOR_STATION].push_back(pInteract);
		break;
	}
}

void CRealPlayer::Set_PlayerFirstPos(_float x, _float y, _float z)
{
	if (!m_pTransformCom) {
		MSG_BOX("Player First Pos Set Failed");
		return;
	}
	m_pTransformCom->Set_Pos(x, y, z);
}

void CRealPlayer::Play_StationEffect(CURSOR_ID eID, const _tchar* EffectName)
{
	if (CURSOR_STATION == eID) {
		if (m_pIChop) {
			Engine::CEffectMgr::GetInstance()->Play_Effect(EffectName, dynamic_cast<CChopStation*>(m_pIChop));
		}
	}

}

CGameObject* CRealPlayer::Get_GrabObj()
{
	if (m_pGrabObj)
		return m_pGrabObj;

	return nullptr;
}

void CRealPlayer::KeyInput()
{
	// 1P키
	if (m_ePlayerNum == PLAYER_1P && CDInputMgr::GetInstance()->Get_DIKeyState(DIK_X) & 0x80)
	{
		if (m_bKeyCheck[DIK_X]) return;
		m_bKeyCheck[DIK_X] = true;
		//--------------- Body ---------------//
		GrabKey_Algorithm();
	}
	else m_bKeyCheck[DIK_X] = false;

	if (m_ePlayerNum == PLAYER_1P && dynamic_cast<CFireExtinguisher*>(m_pGrabObj) && CDInputMgr::GetInstance()->Get_DIKeyState(DIK_Z)) {
		ActKey_Extinguish();
	}


	if (m_ePlayerNum == PLAYER_1P && CDInputMgr::GetInstance()->Get_DIKeyState(DIK_Z) & 0x80)
	{
		if (m_bKeyCheck[DIK_Z]) return;
		m_bKeyCheck[DIK_Z] = true;
		//--------------- Body ---------------//
		ActKey_Algorithm();
	}
	else m_bKeyCheck[DIK_Z] = false;
		

	// 2P키
	if (m_ePlayerNum == PLAYER_2P && CDInputMgr::GetInstance()->Get_DIKeyState(DIK_PERIOD) & 0x80)
	{
		if (m_bKeyCheck[DIK_PERIOD]) return;
		m_bKeyCheck[DIK_PERIOD] = true;
		//--------------- Body ---------------//
		GrabKey_Algorithm();
	}
	else m_bKeyCheck[DIK_PERIOD] = false;

	if (m_ePlayerNum == PLAYER_2P && dynamic_cast<CFireExtinguisher*>(m_pGrabObj) && CDInputMgr::GetInstance()->Get_DIKeyState(DIK_COMMA)) {
		ActKey_Extinguish();
	}

	if (m_ePlayerNum == PLAYER_2P && CDInputMgr::GetInstance()->Get_DIKeyState(DIK_COMMA) & 0x80)
	{
		if (m_bKeyCheck[DIK_COMMA]) return;
		m_bKeyCheck[DIK_COMMA] = true;
		//--------------- Body ---------------//
		ActKey_Algorithm();
	}
	else m_bKeyCheck[DIK_COMMA] = false;
	

	//---------------------- 테스트용 ----------------------//
	//---------------------- 모션 확인용 ----------------------//
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_LBRACKET) & 0x80)
	{
		if (m_bKeyCheck[DIK_LBRACKET]) return;
		m_bKeyCheck[DIK_LBRACKET] = true;
		//--------------- Body ---------------//
		if (m_bTestAct[ACT_CHOP]) return;
		Change_HandState("Chop");
		Change_PlayerState("Player_Act");
		m_bTestAct[ACT_CHOP] = true;
		test[0] = 0;

	}
	else m_bKeyCheck[DIK_LBRACKET] = false;

	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_RBRACKET) & 0x80)
	{
		if (m_bKeyCheck[DIK_RBRACKET]) return;
		m_bKeyCheck[DIK_RBRACKET] = true;
		//--------------- Body ---------------//
		if (m_bTestAct[ACT_WASH]) return;
		Change_HandState("Wash");
		Change_PlayerState("Player_Act");
		m_bTestAct[ACT_WASH] = true;
		test[1] = 0;

	}
	else m_bKeyCheck[DIK_RBRACKET] = false;

	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_P) & 0x80)
	{
		if (m_bKeyCheck[DIK_P]) return;
		m_bKeyCheck[DIK_P] = true;
		//--------------- Body ---------------//
		//if (!m_bTestAct[ACT_SURPRISED]) {
		//	Change_HandState("Surprised");
		//	m_bTestAct[ACT_SURPRISED] = true;
		//	test[3] = 0;
		//}
		//else {
		//	Change_HandState("Idle");
		//	m_bTestAct[ACT_SURPRISED] = false;
		//	for (auto& pHand : m_vecHands) {
		//		pHand->Set_Surprised(false);
		//	}
		//}
		Start_SurprisedAnimaition();
	}
	else m_bKeyCheck[DIK_2] = false;

	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_2) & 0x80)
	{
		if (!m_bExtinguish) {
			Engine::CEffectMgr::GetInstance()->Play_Effect(L"ExtinguishEffect", this);
			m_bExtinguish = true;
			m_fExtinguishCool = 0.f;
		}
	}

}

void CRealPlayer::Reset_Cursor()
{
	if (m_pCursorCarriable) {
		dynamic_cast<CInteract*>(m_pCursorCarriable)->Set_Highlight(false);
	}
	if (m_pCursorStation) {
		dynamic_cast<CInteract*>(m_pCursorStation)->Set_Highlight(false);
	}

	m_pCursorCarriable = nullptr;
	m_pCursorStation = nullptr;
}

void CRealPlayer::Reset_DetectedList()
{
	for (_uint i = CURSOR_ALL; i < CURSOR_END; ++i) {
		CURSOR_ID eID = static_cast<CURSOR_ID>(i);
		m_listDetected[eID].clear();
	}
}

void CRealPlayer::Free()
{
	for (auto pHand : m_vecHands) { Safe_Release(pHand); }
	Engine::CGameObject::Free();
}
