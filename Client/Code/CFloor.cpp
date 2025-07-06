#include "pch.h"
#include "CFloor.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CFloor::CFloor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CFloor::CFloor(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

CFloor::~CFloor()
{
}

HRESULT CFloor::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale({15.f, 15.f, 1.f});
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));
	_vec3 vScale = m_pTransformCom->Get_Scale();
	m_pTransformCom->Set_Pos(vScale.x * 0.5f, 0.001f, vScale.y * 0.5f);

    return S_OK;
}

_int CFloor::Update_GameObject(const _float& fTimeDelta)
{
    int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CFloor::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CFloor::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(1);

	if (FAILED(Set_Material()))
		return;

	m_pBufferCom->Render_Buffer();
}

HRESULT CFloor::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_EnvironmentTexture_Floor"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

    return S_OK;
}

CFloor* CFloor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CFloor* pFloor = new CFloor(pGraphicDev);

    if (FAILED(pFloor->Ready_GameObject()))
    {
        Safe_Release(pFloor);
        MSG_BOX("Floor Create Failed");
        return nullptr;
    }

    return pFloor;
}

void CFloor::Free()
{
    Engine::CGameObject::Free();
}