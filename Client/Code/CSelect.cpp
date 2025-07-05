#include "pch.h"
#include "CSelect.h"
#include "CFontMgr.h"
#include "CUi_Factory.h"
#include "CUi_TimeLimit.h"

CSelect::CSelect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CSelect::~CSelect()
{
    Free();
}

HRESULT	CSelect::Ready_Scene() {
    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameObject_Layer(L"GameObject_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    return S_OK;
}
_int CSelect::Update_Scene(const _float& fTimeDelta) {
    return 0;
}
void CSelect::LateUpdate_Scene(const _float& fTimeDelta) {

}

void CSelect::Render_Scene()
{
    _vec2 vPos = { 100, 200 };

    CFontMgr::GetInstance()->Render_Font(
        L"Font_Default"
        , L"Select"
        , &vPos
        , D3DXCOLOR(0.f, 0.f, 0.f, 1.f)
    );

}

HRESULT	CSelect::Ready_Environment_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;
    Engine::CGameObject* pGameObject = nullptr;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}
HRESULT	CSelect::Ready_GameObject_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;
    Engine::CGameObject* pGameObject = nullptr;

    m_mapLayer.insert({ pLayerTag, pLayer });
    return S_OK;
}
HRESULT	CSelect::Ready_UI_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;
    Engine::CGameObject* pGameObject = nullptr;

    ///////////////////////////////////////////////////////////////////////////////////// UI_Object
    pGameObject = CUi_Factory<CUi_TimeLimit>::Ui_Create(m_pGraphicDev, TIMER_OBJECT);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_TimeLimit>::Ui_Create(m_pGraphicDev, TIMEGAUGE_OBJECT);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object2", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_TimeLimit>::Ui_Create(m_pGraphicDev, TIMEFONT_OBJECT);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object3", pGameObject)))
        return E_FAIL;

    ////////////////////////////////////////////////////////////////////////////////UI_Button
    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, STORY_BUTTON);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button0", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, STORY_BUTTON);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button1", pGameObject)))
        return E_FAIL;


    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, ARCADE_BUTTON);
    //이케이드 버튼
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button2", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, ARCADE_BUTTON);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button3", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, BATTLE_BUTTON);
    //대전 버튼
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button4", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, BATTLE_BUTTON);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button5", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, CHEF_BUTTON);
    //요리사 버튼
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button6", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, CHEF_BUTTON);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button7", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, STATE_BUTTON);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button8", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, STATE_BUTTON);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button9", pGameObject)))
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });
    return S_OK;
}

HRESULT	CSelect::Ready_Prototype() {
    return S_OK;
}

CSelect* CSelect::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
    CSelect* pSelect = new CSelect(pGraphicDev);

    if (FAILED(pSelect->Ready_Scene())) {
        Safe_Delete(pSelect);
        MSG_BOX("Select Create Failed");
        return nullptr;
    }

    return pSelect;
}

void CSelect::Free() {

}