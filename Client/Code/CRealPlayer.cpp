#include "pch.h"
#include "CRealPlayer.h"

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
	return S_OK;
}

HRESULT CRealPlayer::Ready_GameObject() 
{
	return S_OK;
}

_int CRealPlayer::Update_GameObject(const _float& fTimeDelta)
{
	return S_OK;
}

void CRealPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CRealPlayer::Render_GameObject()
{

}

CRealPlayer* CRealPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
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
