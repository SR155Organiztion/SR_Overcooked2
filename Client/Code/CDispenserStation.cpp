#include "pch.h"
#include "CDispenserStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CIngredient.h"
#include "CObjectPoolMgr.h"
#include "CManagement.h"
#include "CSoundMgr.h"

CDispenserStation::CDispenserStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CDispenserStation::~CDispenserStation()
{
}

HRESULT	CDispenserStation::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale({ 1.f, 1.f, 1.f });
	m_pTransformCom->Set_Pos(12.5f, 1.2f, 4.5f);
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-90.f));

	m_stOpt.bApplyGravity = false;
	m_stOpt.bApplyCollision = false;
	//m_stOpt.bApplyRolling = false;
	//m_stOpt.bApplyBouncing = false;
	//m_stOpt.bIsStation = true;
	//m_stOpt.eBoundingType = BOX;
	//m_stOpt.stCollisionOpt = AABB;

	return S_OK;
}

_int CDispenserStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	Check_FirstPause(fTimeDelta);

	Check_ThrowCool(fTimeDelta);
	Throw_Ingredient();

	return S_OK;

}

void CDispenserStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CDispenserStation::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

HRESULT	CDispenserStation::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CCubeTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_CubeTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Dispenser"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CDispenserStation::Throw_Ingredient()
{
	if (m_bThrow || CIngredient::ING_END == m_eTypeIngredient || !m_bFirstPause)
		return;

	// 오브젝트 풀에서 재료 꺼내옴
	_vec3 vPos; m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos.z -= 0.6f; //디스펜서 z축으로 살짝 빼서 소환
	vPos.x -= 0.2f;
	CGameObject* pIngredient = CObjectPoolMgr::GetInstance()->Get_Object(m_szIngredientName, vPos);
	if (!pIngredient)
		return ;
	// 꺼낸 재료 레이어에 등록
	dynamic_cast<CIngredient*>(pIngredient)->Init();
	CManagement::GetInstance()->Get_Layer(L"GameObject_Layer")->Add_GameObject(pIngredient->Get_SelfId(), pIngredient);
	// Dispenser의 Look벡터 추출
	_vec3 vLook; m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	// 던지기
	CInteract* pBeThrownIngredient = dynamic_cast<CInteract*>(pIngredient);
	if (m_bFar) {
		pBeThrownIngredient->Be_Thrown(vLook, 12.f);
		m_bFar = false;
	}
	else { // m_bFar == false;
		pBeThrownIngredient->Be_Thrown(vLook, 5.f);
		m_bFar = true;
	}
	pBeThrownIngredient->Set_Ground(false);
	// 쿨타임 시작
	m_bThrow = true;
	m_fThrowCool = 0.f;
	
	CSoundMgr::GetInstance()->Play_Sound(INGAME_DISPENSER, INGAME_SFX_CHANNEL);
}

void CDispenserStation::Check_ThrowCool(const _float& dt)
{
	if (!m_bThrow)
		return;

	if (m_bThrow) {
		m_fThrowCool += dt;
		if (m_fThrowTick < m_fThrowCool) {
			m_bThrow = false;
			m_fThrowCool = 0.f;
		}
	}
}

void CDispenserStation::Check_FirstPause(const _float& dt)
{
	if (m_bFirstPause)
		return;

	m_fFirstPause += dt;
	if (5.f < m_fFirstPause) {
		m_bFirstPause = true;
	}
}

CDispenserStation* CDispenserStation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDispenserStation* pStation = new CDispenserStation(pGraphicDev);

	if (FAILED(pStation->Ready_GameObject()))
	{
		Safe_Release(pStation);
		MSG_BOX("Station_Dispenser Failed");
		return nullptr;
	}

	return pStation;
}

void CDispenserStation::Set_TypeIngredientStation(const _tchar* create_name)
{
	if (_tcscmp(create_name, L"Dispenser_Rice") == 0) {
		m_eTypeIngredient = CIngredient::RICE;
		m_szIngredientName = L"Rice";
		}
	else if (_tcscmp(create_name, L"Dispenser_Pasta") == 0) {
		m_eTypeIngredient = CIngredient::PASTA;
		m_szIngredientName = L"Pasta";
	}
	else {
		MSG_BOX("Dispenser Station Name Set Failed... reason : Cant Find correct Name");
		return;
	}
}

void CDispenserStation::Free()
{
	Engine::CGameObject::Free();
}