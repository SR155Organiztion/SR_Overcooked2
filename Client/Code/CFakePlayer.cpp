#include "pch.h"
#include "CFakePlayer.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CTransform.h"
#include "CLayer.h"
#include "CFontMgr.h"

#include "CPlayerHand.h"

#include "IPlace.h"
#include "IChop.h"
#include "CGasStation.h"
#include "CManagement.h"
#include "CIngredientStation.h"
#include "CEffectMgr.h"

CFakePlayer::CFakePlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_ePlayerNum(PLAYERNUM_END), m_bKeyCheck{}, m_bAct{}
	, m_pCursorCarriable(nullptr), m_pCursorStation(nullptr), m_pGrabObj(nullptr), m_pIChop(nullptr), m_pActStation(nullptr), m_strCurName{}
	, test{}, m_szShowTestTime{}, m_bTestAct{}
{
}

CFakePlayer::CFakePlayer(const CGameObject& rhs)
	: Engine::CGameObject(rhs)
	, m_ePlayerNum(PLAYERNUM_END), m_bKeyCheck{}
	, m_pCursorCarriable(nullptr), m_pCursorStation(nullptr), m_pGrabObj(nullptr), m_pIChop(nullptr), m_pActStation(nullptr), m_strCurName{}
{
}

CFakePlayer::~CFakePlayer()
{
}

HRESULT CFakePlayer::Add_Component()
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

HRESULT CFakePlayer::Ready_Hands()
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

HRESULT CFakePlayer::Ready_GameObject()
{
	if (FAILED(Add_Component())) 
		return E_FAIL;
	if (FAILED(Ready_Hands())) 
		return E_FAIL;

	m_pFSMCom->Add_State("Player_Idle", new CPlayerIdle);
	m_pFSMCom->Add_State("Player_Move", new CPlayerMove);
	m_pFSMCom->Add_State("Player_Act", new CPlayerAct);
	m_pFSMCom->Change_State("Player_Idle");

	m_pTransformCom->m_vScale = { 1.f, 2.f, 1.f };
	//m_pTransformCom->Set_Pos(8.f, 1.f, 5.f);
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f));

	m_ePlayerNum = PLAYER_1P; // 2p ������ ���� ��������

	//m_stOpt.bApplyGravity = false;
	m_stOpt.bApplyGravity = true;
	//m_stOpt.bApplyRolling = true;
	//m_stOpt.bApplyBouncing = false;
	//m_stOpt.bApplyKnockBack = true;
	m_stOpt.bPushable = true;



	return S_OK;
}

_int CFakePlayer::Update_GameObject(const _float& fTimeDelta)
{
	Reset_Cursor();
	Check_Act(fTimeDelta);
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	Update_Hands(fTimeDelta);
	Set_Cursor();
	Set_GrabObjMat();
	KeyInput();
	Check_CursorName();
	Reset_DetectedList();

	return S_OK;
}

void CFakePlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CFakePlayer::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	Engine::CGameObject::Set_Material();

	m_pBufferCom->Render_Buffer();

	for (auto& pHand : m_vecHands) {
		pHand->Render_GameObject();
	}

	//Render_TestName();

}

void CFakePlayer::Check_Act(const _float& dt)
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
		swprintf_s(m_szShowTestTime, L"Chop %f", test[0]);	// �׽�Ʈ
		if (5 <= test[0]) {
			Escape_Act(ACT_CHOP, false);
			m_bTestAct[ACT_CHOP] = false;
		}
	}
	if (m_bTestAct[ACT_WASH]) {
		test[1] += dt;
		swprintf_s(m_szShowTestTime, L"Wash %f", test[1]);	// �׽�Ʈ
		if (5 <= test[1]) {
			Escape_Act(ACT_WASH, true);
			m_bTestAct[ACT_WASH] = false;
		}
	}
}

void CFakePlayer::Shine_Cursor(CGameObject* pCursor)
{
	if (!pCursor)
		return;
	dynamic_cast<CInteract*>(pCursor)->Set_Highlight(true);
}

void CFakePlayer::Check_CursorName()
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
				m_strCurName[CURSOR_STATION] = L"Ingredient_Station";
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
		if (dynamic_cast<IPlace*>(m_pCursorStation)->Is_Full()) {
			m_strCurName[CURSOR_STATION_ON_ITEM] = L"On_item";
		}
		else m_strCurName[CURSOR_STATION_ON_ITEM] = L"";

	}
	else m_strCurName[CURSOR_STATION] = L"";

}

void CFakePlayer::Render_TestName()
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

CFakePlayer* CFakePlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFakePlayer* pPlayer = new CFakePlayer(pGraphicDev);

	if (FAILED(pPlayer->Ready_GameObject())) {
		Safe_Release(pPlayer);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}
	return pPlayer;
}

CGameObject* CFakePlayer::Find_Cursor(CURSOR_ID eID)
{
	if (CURSOR_ALL > eID || CURSOR_END < eID) return nullptr;

	if (!m_listDetected[eID].empty()) return m_listDetected[eID].front();

	return nullptr;
}

void CFakePlayer::Update_Hands(const _float dt)
{
	for (auto& pHand : m_vecHands) {
		pHand->Update_GameObject(dt);
	}
}

void CFakePlayer::Set_Cursor()
{
	if (nullptr == m_pGrabObj) { //������ ������, Ŀ���� �ƹ��ų� ����Ŵ
		m_pCursorCarriable = Find_Cursor(CURSOR_ALL);
		Shine_Cursor(m_pCursorCarriable);
	}
	else {
		CInteract* pGrab = dynamic_cast<CInteract*>(m_pGrabObj);
		if (pGrab) {//���� ���������
			CInteract::INTERACTTYPE eID = pGrab->Get_InteractType();
			switch (eID) {
			case CInteract::INGREDIENT: //�װ� ����϶� ������ ����Ŵ
				m_pCursorCarriable = Find_Cursor(CURSOR_TOOL);
				Shine_Cursor(m_pCursorCarriable);
				break;
			case CInteract::FRYINGPAN: //�װ� �������� ��, ���� ���ø� ����Ŵ
			case CInteract::POT:
				m_pCursorCarriable = Find_Cursor(CURSOR_NOTOOL);
				Shine_Cursor(m_pCursorCarriable);
				break;
			case CInteract::PLATE: // ����ִ°� ������ ��, ��� ����Ŵ
				m_pCursorCarriable = Find_Cursor(CURSOR_ALL);
				Shine_Cursor(m_pCursorCarriable);
				break;
			}
		}
	}
	m_pCursorStation = Find_Cursor(CURSOR_STATION);
	if (m_pCursorStation) Shine_Cursor(m_pCursorStation);
}

void CFakePlayer::Set_GrabObjMat()
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
	pGrabObj_TransCom->Set_Pos(vecObjPos.x, vecObjPos.y + 0.2f, vecObjPos.z);
}

void CFakePlayer::Drop_GrabObject()
{
	Change_HandState("Idle");
	dynamic_cast<CInteract*>(m_pGrabObj)->Set_Ground(false);
	m_pGrabObj = nullptr;
}

void CFakePlayer::Change_HandState(std::string newState)
{
	std::string LeftState = "LeftHand_" + newState;
	std::string RightState = "RightHand_" + newState;
	dynamic_cast<CFSMComponent*>(m_vecHands[0]->Get_Component(ID_DYNAMIC, L"Com_FSM"))->Change_State(LeftState);
	dynamic_cast<CFSMComponent*>(m_vecHands[1]->Get_Component(ID_DYNAMIC, L"Com_FSM"))->Change_State(RightState);
}

void CFakePlayer::Escape_Act(ACT_ID eID, _bool IsPause, std::string PlayerState)
{
	if (IsPause) {
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
				if (IsPause) m_pIChop->Pause_Process();
				m_pActStation = nullptr;
				m_pIWash = nullptr;
			}
			//dynamic_cast<CPlayerHand*>(m_vecHands[1])->Set_UseVirtaulPivot(false); //�ӽ�
			//test[0] = 0;
			break;
		}
	}
	m_bTestAct[eID] = false; //�׽�Ʈ
	m_bAct[eID] = false;
	Change_HandState("Idle");
	m_pFSMCom->Change_State(PlayerState);
}

void CFakePlayer::Change_PlayerState(std::string PlayerState)
{
	m_pFSMCom->Change_State(PlayerState);
}

CGameObject* CFakePlayer::Get_CursorStation()
{
	if (!m_pActStation)
		return nullptr;

	return m_pActStation;
}

void CFakePlayer::On_Detected(CGameObject* _pGameObject)
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

	}
}

void CFakePlayer::On_Collision(CGameObject* _pGameObject)
{
	CInteract* pInteract = dynamic_cast<CInteract*>(_pGameObject);
	if (nullptr == pInteract) return;

	switch (pInteract->Get_InteractType()) {
	case CInteract::STATION:
	case CInteract::CHOPSTATION:
	case CInteract::SINKSTATION:
	case CInteract::EMPTYSTATION:
		m_listDetected[CURSOR_STATION].push_back(pInteract);
		break;
	}
}

void CFakePlayer::Set_PlayerFirstPos(_float x, _float y, _float z)
{
	if (!m_pTransformCom) {
		MSG_BOX("Player First Pos Set Failed");
		return;
	}
	m_pTransformCom->Set_Pos(x, y, z);
}

void CFakePlayer::KeyInput()
{
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_SPACE) & 0x80)
	{
		if (m_bKeyCheck[DIK_SPACE]) return;
		m_bKeyCheck[DIK_SPACE] = true;
		//--------------- Body ---------------//
		if (m_pGrabObj) { // �տ� ����ִ��� Ȯ��
			if (m_pCursorStation) { //��� ���� ��, stationĿ�� �ִ��� Ȯ��
				IPlace* pStation = dynamic_cast<IPlace*>(m_pCursorStation); // Station ���ϰ� ������ �̸� �ٿ�ĳ����
				CInteract* pStationOnItem = dynamic_cast<CInteract*>(pStation->Get_Item()); // Station�� �������ִ��� �̸� ����. ����ó�� �� �ϰ� ����

				CInteract::INTERACTTYPE eGrab = dynamic_cast<CInteract*>(m_pGrabObj)->Get_InteractType(); // �տ� ������ ����
				switch (eGrab) {
				case CInteract::INGREDIENT: // �տ� ������ ���
					if (pStationOnItem) { // Station�� ������ �ִٸ�
						if (CInteract::INGREDIENT != pStationOnItem->Get_InteractType()) { // Station���� ������ �ִٸ�
							if (dynamic_cast<IPlace*>(pStation->Get_Item())->Set_Place(m_pGrabObj, pStation->Get_Item())) { // Station���� ������ �ִٸ� �տ� ����ִ� ��Ḧ ������ �ִ� �õ�
								m_pGrabObj = nullptr;
								Change_HandState("Idle");
							}
						}
					}
					else { //Station�� ������ ���ٸ�
						if (pStation->Set_Place(m_pGrabObj, m_pCursorStation)) { //Station���� ���� ���ٸ� �տ� ����ִ� ��Ḧ station���� �ø��� �õ�
							m_pGrabObj = nullptr;
							Change_HandState("Idle");
						}
					}
					break;
				case CInteract::FRYINGPAN:	//@@@@@@@@@@@@@   Station �� ������ �տ� �� ���� ��ȣ�ۿ� !����! ��������
				case CInteract::POT:		// �տ� ������ ������
					if (pStationOnItem) {	// Station�� �������� �ִٸ�
						if (CInteract::INGREDIENT == dynamic_cast<CInteract*>(pStationOnItem)->Get_InteractType()) { // Station���� �����
							dynamic_cast<IPlace*>(m_pGrabObj)->Set_Place(pStation->Get_PlacedItem(), m_pGrabObj);//Station�� ��Ḧ �տ� �� �ı���� �ִ� �õ�
						}
						else if (CInteract::PLATE == dynamic_cast<CInteract*>(pStationOnItem)->Get_InteractType()) { // Station���� ���ö��
							dynamic_cast<IPlace*>(pStation->Get_Item())->Set_Place(m_pGrabObj, pStation->Get_Item()); //Station�� ���ÿ� �տ� �� �ı������ ��Ḧ �ִ� �õ�
						}
					}
					else { //Station�� ������ ���ٸ�
						if (pStation->Set_Place(m_pGrabObj, m_pCursorStation)) { //Station���� ���� ���ٸ� �տ� ����ִ� ������ station���� �ø��� �õ�
							m_pGrabObj = nullptr;
							Change_HandState("Idle");
						}
					}
					break;
				case CInteract::PLATE:
					if (pStationOnItem) { // Station�� ������ �ִٸ�
						if (CInteract::INGREDIENT == dynamic_cast<CInteract*>(pStation->Get_Item())->Get_InteractType()) { // Station�� ������ �����
							dynamic_cast<IPlace*>(m_pGrabObj)->Set_Place(pStation->Get_PlacedItem(), m_pGrabObj); // station�� ��Ḧ ���ÿ� �ø��� �õ�
						}
						else if (CInteract::INGREDIENT != dynamic_cast<CInteract*>(pStation->Get_Item())->Get_InteractType()) { // Station�� ������ �ı�� or ���ö��
							dynamic_cast<IPlace*>(m_pGrabObj)->Set_Place(dynamic_cast<IPlace*>(pStation->Get_Item())->Get_PlacedItem(), m_pGrabObj); // station�� �ı�� or ���ÿ� �ִ� ��Ḧ ���ÿ� �ø��� �õ�
						}
					}
					else { //station�� �ƹ��͵� ���ٸ�
						if (pStation->Set_Place(m_pGrabObj, m_pCursorStation)) { //Station����  ���ٸ� �տ� ����ִ� ���ø��� station���� �ø��� �õ�
							m_pGrabObj = nullptr;
							Change_HandState("Idle");
						}
					}
				}
			}
			else {// m_pCursorStation�� ���ٸ�
				if (m_pCursorCarriable) {
					CInteract::INTERACTTYPE eID = dynamic_cast<CInteract*>(m_pGrabObj)->Get_InteractType(); // ���� ������ ID Ȯ��
					switch (eID) {
					case CIngredient::INGREDIENT: // ��� �ִ°� ����� 
					{
						IPlace* pTool = dynamic_cast<IPlace*>(m_pCursorCarriable);
						if (pTool) { // Ŀ���� �������
							if (pTool->Set_Place(m_pGrabObj, m_pCursorCarriable)) { // �տ� ��Ḧ ��� �ְ� ������ Ŀ���� �����ٸ� ������ �ִ� �õ�
								m_pGrabObj = nullptr;
								Change_HandState("Idle");
							}
						}
					}
					break;
					case CIngredient::FRYINGPAN:
					case CIngredient::POT: //��� �ִ°� �������
					{
						if (CInteract::INGREDIENT == dynamic_cast<CInteract*>(m_pCursorCarriable)->Get_InteractType()) { // Ŀ���� �����
							dynamic_cast<IPlace*>(m_pGrabObj)->Set_Place(m_pCursorCarriable, m_pGrabObj);//�տ��� ��Ḧ �տ� �� �ı���� �ִ� �õ�
						}
						else if (CInteract::PLATE == dynamic_cast<CInteract*>(m_pCursorCarriable)->Get_InteractType()) { // Ŀ���� ���ö��
							dynamic_cast<IPlace*>(m_pCursorCarriable)->Set_Place(m_pGrabObj, m_pCursorCarriable); //Ŀ���� ���� ���ÿ� �տ� �� �ı������ ��Ḧ �ִ� �õ�
						}
					}
					break;
					case CIngredient::PLATE: // ��� �ִ°� ���ö��
					{
						if (m_pCursorCarriable) { // Ŀ���� �������� ��
							CInteract::INTERACTTYPE CursorID = dynamic_cast<CInteract*>(m_pCursorCarriable)->Get_InteractType(); // Ŀ���� ����?
							switch (CursorID) {
							case CInteract::INGREDIENT: // ����ִ°� ���ð� Ŀ���� ��ᰡ ������
								dynamic_cast<IPlace*>(m_pGrabObj)->Set_Place(m_pCursorCarriable, m_pGrabObj);
								break;
							case CIngredient::FRYINGPAN:
							case CIngredient::POT:
							case CIngredient::PLATE: //��� �ִ°� ���ð� Ŀ���� ������ ������
								dynamic_cast<IPlace*>(m_pGrabObj)->Set_Place(m_pCursorCarriable, m_pGrabObj);
							}
						}
						else
							Drop_GrabObject(); // ����ִ� ��ü�� ��ȣ�ۿ��� Ŀ���� ������ �տ��� ����
					}
					break;
					}
				}
				else {
					Drop_GrabObject(); // ����ִ� ��ü�� ��ȣ�ۿ��� Ŀ���� ������ �տ��� 
				}
			}
		}
		else { // �տ� ����ִ°� ���ٸ�
			// ��ó�� ���� �� �ִ� �繰 Ž�� �� Ŀ��(m_pCursorCarriable) Ž��
			if (m_pCursorCarriable) { // m_pCursorCarriableĿ���� ���� �ٸ�, 
				m_pGrabObj = m_pCursorCarriable; // Ŀ���� ��� ��ü�� 
				m_pCursorCarriable = nullptr; // Ŀ�� �����
				dynamic_cast<CInteract*>(m_pGrabObj)->Set_Ground(true); // ��� �ִ� ��ü �߷� ����
				// ���� State ��ȯ
				Change_HandState("Grab");
				//���� �Լ� �߰����� (����� �˹�, �Ѹ� ���� �Լ�)
			}
			else { // ������ Ŀ���� ���ٸ�
				if (m_pCursorStation) { //�ٵ� �����̼� Ŀ���� �ִٸ�?
					m_pGrabObj = dynamic_cast<IPlace*>(m_pCursorStation)->Get_PlacedItem(); // �����̼ǿ� ������Ʈ�� �ִٸ� ��������
					CIngredientStation* pIngrediStation = dynamic_cast<CIngredientStation*>(m_pCursorStation);
					if (m_pGrabObj) {
						Change_HandState("Grab");				//���� �Լ� �߰����� (����� �˹�, �Ѹ� ���� �Լ�)
						dynamic_cast<CInteract*>(m_pGrabObj)->Set_Ground(true); // ��� �ִ� ��ü �߷� ����
					}
					else if (!m_pGrabObj && pIngrediStation) {
						CGameObject* pIngre = pIngrediStation->TakeOut_Ingredient();
						if (pIngre) {
							m_pGrabObj = pIngre;
							Change_HandState("Grab");				//���� �Լ� �߰����� (����� �˹�, �Ѹ� ���� �Լ�)
							dynamic_cast<CInteract*>(m_pGrabObj)->Set_Ground(true); // ��� �ִ� ��ü �߷� ����
						}
					}
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
		if (m_pGrabObj) {
			CInteract::INTERACTTYPE eGrab = dynamic_cast<CInteract*>(m_pGrabObj)->Get_InteractType();
			if (eGrab == CInteract::INGREDIENT) {
				_vec3 vLook;
				m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
				CInteract* pInteract = dynamic_cast<CInteract*>(m_pGrabObj);
				pInteract->Be_Thrown(vLook, 10.f);
				pInteract->Set_Ground(false);
				m_pGrabObj = nullptr;
				Change_HandState("Throw");
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
					Change_HandState("Wash");
					m_pFSMCom->Change_State("Player_Act");
					m_pActStation = m_pCursorStation;
					m_bAct[ACT_WASH] = true;
				}
			}
		}
	}
	else m_bKeyCheck[DIK_LCONTROL] = false;



	//---------------------- �׽�Ʈ�� ----------------------//
	//---------------------- ��� Ȯ�ο� ----------------------//
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



}

void CFakePlayer::Reset_Cursor()
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

void CFakePlayer::Reset_DetectedList()
{
	for (_uint i = CURSOR_ALL; i < CURSOR_END; ++i) {
		CURSOR_ID eID = static_cast<CURSOR_ID>(i);
		m_listDetected[eID].clear();
	}
}

void CFakePlayer::Free()
{
	for (auto pHand : m_vecHands) { Safe_Release(pHand); }
	Engine::CGameObject::Free();
}
