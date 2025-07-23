#include "pch.h"
#include "CFlag.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
//플레이어말고 차 생기면 이름바꿀것
#include "CRealPlayer.h"
#include "CBus.h"
#include <CSelectGameSystem.h>
#include <CStageLoading.h>

CFlag::CFlag(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CFlag::CFlag(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CFlag::~CFlag()
{
}

HRESULT CFlag::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    //생성시 클리어가 되지 않은 깃발은 자동으로 4
    m_iStarNum = 4;

    return S_OK;
}

_int CFlag::Update_GameObject(const _float& fTimeDelta)
{

    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    _matrix matWorld, matView, matBill;

    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixIdentity(&matBill);
    D3DXMatrixInverse(&matView, 0, &matView);

    _vec3	vViewScale, vViewTrans;
    D3DXQUATERNION qViewRot;
    D3DXMatrixDecompose(&vViewScale, &qViewRot, &vViewTrans, &matView);
    _matrix matViewRot;  D3DXMatrixRotationQuaternion(&matViewRot, &qViewRot);

    matWorld = matViewRot * matWorld;

    m_pTransformCom->m_matWorld = matWorld;

    {
        CGameObject* pPlayer = CManagement::GetInstance()->Get_GameObject(L"GameObject_Layer", L"Bus");

        if (!pPlayer) { return -1; }
        CComponent* pPlayerTransCom = pPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform");
        _vec3 vPlayerPos;
        dynamic_cast<CTransform*>(pPlayerTransCom)->Get_Info(INFO_POS, &vPlayerPos);
        _vec3 vDistance = m_pTransformCom->m_vInfo[INFO_POS] - vPlayerPos;
        if (D3DXVec3Length(&vDistance) < 1.2f) {
            //충돌시 명령어 V            
            if (GetAsyncKeyState(VK_SPACE)) {
                string szStageKey = "Stage" + to_string(m_iStageNum + 1);

                CScene* pScene = CStageLoading::Create(m_pGraphicDev, szStageKey);
                CSelectGameSystem::GetInstance()->Set_CurStageNum(m_iStageNum);
                if (nullptr == pScene)
                    return -1;

                if (FAILED(CManagement::GetInstance()->Go_Stage(pScene)))
                    return -1;
                else
                    return -1;
            }
        }
    }
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CFlag::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Engine::CGameObject::Compute_ViewZ(&vPos);

    return;
}

void CFlag::Render_GameObject()
{
    D3DXMATRIX matWorld;
    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    m_pTextureCom->Set_Texture(m_iStarNum);

    if (FAILED(Set_Metarial()))
        return;

    m_pBufferCom->Render_Buffer();

    // m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CFlag::Set_Star(_uint _iID)
{
    m_iStarNum = _iID;
}

void CFlag::Set_Angle(const _float _fAngle)
{
    m_pTransformCom->m_vAngle.y = _fAngle;
}

void CFlag::Set_Scale(const _vec3 _vScale)
{
    m_pTransformCom->Set_Scale(_vScale);
}


void CFlag::Set_StageName(string _s)
{
    m_szStage = _s;
}

_vec3 CFlag::Get_Pos()
{
    return m_pTransformCom->m_vInfo[INFO_POS];
}

HRESULT CFlag::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_EnvironmentObject_Map_Flag"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


    return S_OK;
}

HRESULT CFlag::Set_Metarial()
{
    D3DMATERIAL9 tMetarial;
    ZeroMemory(&tMetarial, sizeof(D3DMATERIAL9));

    tMetarial.Diffuse = D3DXCOLOR(1.f, 0.f, 1.f, 1.f);
    tMetarial.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tMetarial.Ambient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.6f);

    tMetarial.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
    tMetarial.Power = 0.f;

    m_pGraphicDev->SetMaterial(&tMetarial);

    return S_OK;
}

CFlag* CFlag::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CFlag* pFlag = new CFlag(pGraphicDev);

    if (FAILED(pFlag->Ready_GameObject()))
    {
        Safe_Release(pFlag);
        MSG_BOX("pFlag Create Failed");
        return nullptr;
    }

    return pFlag;
}

void CFlag::Free()
{
    Engine::CGameObject::Free();
}
