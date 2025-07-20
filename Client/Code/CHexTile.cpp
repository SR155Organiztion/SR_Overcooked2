#include "pch.h"
#include "CHexTile.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"

CHexTile::CHexTile(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CHexTile::CHexTile(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CHexTile::~CHexTile()
{
}

HRESULT CHexTile::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_bFliped = true;
    memset(m_bAction, 0, sizeof(m_bAction));
    m_fHeight = m_pTransformCom->m_vInfo[INFO_POS].y + 0.2f;

    return S_OK;
}

_int CHexTile::Update_GameObject(const _float& fTimeDelta)
{
    DoFlip(fTimeDelta);

    if (GetAsyncKeyState('I')) {
        m_bFliped = false;
    }

    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CHexTile::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    return;
}

void CHexTile::Render_GameObject()
{
    D3DXMATRIX matWorld;
    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pTextureCom->Set_Texture(m_iTextureNum);

    if (FAILED(Set_Metarial()))
        return;

    //±âµÕ Ãâ·Â
    m_pTextureCom->Set_Texture(0);
    m_pPrismBufferCom->Render_Buffer();

    //¹Ù´Ú Ãâ·Â
    m_pTextureCom->Set_Texture(1);
    m_pTileBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CHexTile::Set_TextureNum(_uint _iID)
{
    m_iTextureNum = _iID;
}

void CHexTile::Flip()
{
    m_bFliped = false;
}

void CHexTile::DoFlip(const _float& fTimeDelta)
{
    if (!m_bFliped) {

        if (!m_bAction[0]) {
            m_pTransformCom->m_vAngle.x += D3DXToRadian(-500.f) * fTimeDelta;
            m_pTransformCom->m_vInfo[INFO_POS].y += 0.1f * fTimeDelta;
            if (m_pTransformCom->m_vAngle.x <= D3DXToRadian(-180.f)) {
                m_pTransformCom->m_vAngle.x = D3DXToRadian(180.f);
                m_bAction[0] = true;
            }

            return;
        }

        if (!m_bAction[1]) {
            m_pTransformCom->m_vInfo[INFO_POS].y += 1.f * fTimeDelta;
            if (m_pTransformCom->m_vInfo[INFO_POS].y >= m_fHeight) {
                m_bAction[1] = true;
            }

            return;
        }

        if (!m_bAction[2]) {
            m_pTransformCom->m_vInfo[INFO_POS].y -= 1.5f * fTimeDelta;
            if (m_pTransformCom->m_vInfo[INFO_POS].y <= (- 0.25f)) {
                m_pTransformCom->m_vInfo[INFO_POS].y = -0.25f;
                m_bAction[2] = true;
            }

            return;
        }
    }
}

HRESULT CHexTile::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pTileBufferCom = dynamic_cast<Engine::CHexTileTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_HexTileTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pPrismBufferCom = dynamic_cast<Engine::CHexPrismTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_HexPrismTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_HexTileTexture"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    return S_OK;
}

HRESULT CHexTile::Set_Metarial()
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

CHexTile* CHexTile::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CHexTile* pHexTile = new CHexTile(pGraphicDev);

    if (FAILED(pHexTile->Ready_GameObject()))
    {
        Safe_Release(pHexTile);
        MSG_BOX("pHexTile Create Failed");
        return nullptr;
    }

    return pHexTile;
}

void CHexTile::Free()
{
    Engine::CGameObject::Free();
}
