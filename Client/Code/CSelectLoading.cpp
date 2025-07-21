#include "pch.h"
#include "CSelectLoading.h"
#include "CFontMgr.h"
#include "CLoading.h"
#include "CSelect.h"
#include "CManagement.h"
#include "CProtoMgr.h"

CSelectLoading::CSelectLoading(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev)
{

}

CSelectLoading::~CSelectLoading()
{
    Free();
}

HRESULT	CSelectLoading::Ready_Scene() {
    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameObject_Layer(L"GameObject_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_SELECT);

    if (m_pLoading == nullptr) {
        return E_FAIL;
    }



    return S_OK;
}
_int CSelectLoading::Update_Scene(const _float& fTimeDelta) {
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);

    if (true == m_pLoading->Get_Finish())
    {
        if (GetAsyncKeyState(VK_RETURN))
        {
            Engine::CScene* pScene = CSelect::Create(m_pGraphicDev);
            if (nullptr == pScene)
                return E_FAIL;

            if (FAILED(CManagement::GetInstance()->Set_Scene(pScene)))
                return E_FAIL;
        }
    }

    return iExit;
}
void CSelectLoading::LateUpdate_Scene(const _float& fTimeDelta) {

}

void CSelectLoading::Render_Scene()
{
    _vec2   vPos{ 100.f, 100.f };

    CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_pLoading->Get_String(), &vPos, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

}

HRESULT	CSelectLoading::Ready_Environment_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}
HRESULT	CSelectLoading::Ready_GameObject_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });
    return S_OK;
}
HRESULT	CSelectLoading::Ready_UI_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    //스테이지 번호 
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
    (L"Proto_SelectNumber", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/StageNumber%d.png", TEX_NORMAL, 6))))
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });
    return S_OK;
}

HRESULT	CSelectLoading::Ready_Prototype() {
    return S_OK;
}

CSelectLoading* CSelectLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
    CSelectLoading* pSelect = new CSelectLoading(pGraphicDev);

    if (FAILED(pSelect->Ready_Scene())) {
        Safe_Delete(pSelect);
        MSG_BOX("SelectLoading Create Failed");
        return nullptr;
    }

    return pSelect;
}

void CSelectLoading::Free() {

}