#include "pch.h"
#include "CRealPlayer.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"

CRealPlayer::CRealPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CRealPlayer::CRealPlayer(const CGameObject& rhs)
	: Engine::CGameObject(rhs)
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

	return S_OK;
}

HRESULT CRealPlayer::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->m_vScale = { 0.5f, 1.f, 0.5f };
	m_pTransformCom->Set_Pos(5.f, 1.f, 5.f);


	return S_OK;
}

_int CRealPlayer::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	return S_OK;
}

void CRealPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CRealPlayer::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
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

void CRealPlayer::Key_Input(const _float& fTimeDelta)
{





}

void CRealPlayer::Rotate_Player(PLAYER_ROT eDir)
{
}

void CRealPlayer::Free()
{
}

void CRealPlayer::Change_State(CState* eState)
{
}

void CRealPlayer::CPlayerIdle::Enter_State(Engine::CGameObject* Obj)
{
}

void CRealPlayer::CPlayerIdle::Update_State(Engine::CGameObject* Obj, const _float& fTimeDelta)
{
}

void CRealPlayer::CPlayerIdle::TestForExit_State(Engine::CGameObject* Obj)
{
}

void CRealPlayer::CPlayerMove::Enter_State(Engine::CGameObject* Obj)
{
}

void CRealPlayer::CPlayerMove::Update_State(Engine::CGameObject* Obj, const _float& fTimeDelta)
{
}

void CRealPlayer::CPlayerMove::TestForExit_State(Engine::CGameObject* Obj)
{
}
