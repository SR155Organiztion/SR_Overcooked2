#include "pch.h"
#include "CLogo.h"
#include "CBackGround.h"
#include "CProtoMgr.h"
#include "CTriCol.h"
#include "CStage.h"
#include "CManagement.h"
#include "CFontMgr.h"
#include "CSelect.h"
#include "CMenu.h"
#include "CSoundMgr.h"

#include "CUi_PostCard.h"
#include "CUi_Factory.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CScene(pGraphicDev), m_pLoading(nullptr)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
    CSoundMgr::GetInstance()->Init();

    if (FAILED(Ready_Prototype()))
        return E_FAIL;


    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    if (FAILED(Ready_Sound()))
        return E_FAIL;

    m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_LOGO, &m_iLoadingPercent);
    if (nullptr == m_pLoading)
        return E_FAIL;

    return S_OK;
}

HRESULT CLogo::Ready_Environment_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    // BackGround

    pGameObject = CBackGround::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"BackGround", pGameObject)))
        return E_FAIL;

    m_mapLayer.insert({pLayerTag, pLayer });

    return S_OK;
}

HRESULT	CLogo::Ready_UI_Layer(const _tchar* pLayerTag) {

    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;
    Engine::CGameObject* pGameObject = nullptr;

    //로고
     pGameObject = CUi_Factory<CUi_PostCard>::Ui_Create(m_pGraphicDev);
     if (nullptr == pGameObject)
         return E_FAIL;
     if (FAILED(pLayer->Add_GameObject(L"Ui_PostCard", pGameObject)))
         return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}


_int CLogo::Update_Scene(const _float& fTimeDelta)
{
    CUi_PostCard* pPostCard = dynamic_cast<CUi_PostCard*>(CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_PostCard"));
    pPostCard->Make_PostCard(true);

    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);
 
    if (true == m_pLoading->Get_Finish())
    {
        Engine::CScene* pScene = CMenu::Create(m_pGraphicDev);
        if (nullptr == pScene)
            return E_FAIL;

        if (FAILED(CManagement::GetInstance()->Set_Scene(pScene)))
            return E_FAIL;
    }

    return iExit;
}

void CLogo::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CLogo::Render_Scene()
{
    _vec2   vPos{ 100.f, 100.f };

    CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_pLoading->Get_String(), &vPos, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

}

HRESULT CLogo::Ready_Sound()
{
    Ready_PlayerSound();

    CSoundMgr::GetInstance()->
        Load_Sound(BGM, "/Frontend.wav", true, BGM_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(BGM_SELECTMAP, "/MapScreen.wav", true, BGM_CHANNEL);

    CSoundMgr::GetInstance()->
        Load_Sound(SELECT_FLIP, "/WorldMapExposed.wav", false, LEVEL_READY_CHANNEL);

    CSoundMgr::GetInstance()->
        Load_Sound(LEVEL_READY1, "/LevelReady_01.wav", false, LEVEL_READY_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(LEVEL_READY2, "/LevelReady_02.wav", false, LEVEL_READY_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(LEVEL_READY3, "/LevelReady_03.wav", false, LEVEL_READY_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(LEVEL_READY4, "/LevelReady_05.wav", false, LEVEL_READY_CHANNEL);

    CSoundMgr::GetInstance()->
        Load_Sound(INGAME_ITEM_PICKUP1, "/Item_PickUp_03.wav", false, INGAME_SFX_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(INGAME_ITEM_PICKUP2, "/Item_PickUp_04.wav", false, INGAME_SFX_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(INGAME_ITEM_PICKUP3, "/Item_PickUp_05.wav", false, INGAME_SFX_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(INGAME_ITEM_PUTDOWN, "/Item_PutDown_01.wav", false, INGAME_SFX_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(INGAME_TRASHCAN, "/TrashCan.wav", false, INGAME_SFX_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(INGAME_HOTPOT_START, "/HotPot_Start.wav", false, INGAME_SFX_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(INGAME_COOKING_COOKED, "/ImCooked.wav", false, INGAME_SFX_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(INGAME_COOKING_WARNING, "/CookingWarning.wav", false, INGAME_SFX_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(INGAME_FIRE_IGNITE, "/FireIgnition1.wav", false, INGAME_SFX_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(INGAME_FIRE_LOOPSTART, "/FireLoopStart.wav", false, INGAME_SFX_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(INGAME_WASHEDPLATE, "/WashedPlate.wav", false, INGAME_SFX_CHANNEL);

    CSoundMgr::GetInstance()->
        Load_Sound(INGAME_HOTPOT_BUBBLE, "/HotPotBubble.wav", true, INGAME_BUBBLE_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(INGAME_HOTPOT_SIZZLE, "/FryingPanSizzle.wav", true, INGAME_SIZZLE_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(INGAME_FIRE_LOOP, "/FireLoop.wav", true, INGAME_FIRE_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(INGAME_EXTINGUISHER_LOOP, "/FireExtinguisherLoop.wav", true, INGAME_EXTINGUISHER_CHANNEL);

    CSoundMgr::GetInstance()->
        Load_Sound(INGAME_DISPENSER, "/Dispenser.wav", false, INGAME_SFX_CHANNEL);

    return S_OK;
}

void CLogo::Ready_PlayerSound()
{
    CSoundMgr::GetInstance()->
        Load_Sound(PLAYER_PICKUP1, "/Item_PickUp_03.wav", false, PLAYER_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(PLAYER_PICKUP2, "/Item_PickUp_04.wav", false, PLAYER_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(PLAYER_PICKUP3, "/Item_PickUp_05.wav", false, PLAYER_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(PLAYER_PUTDOWN, "/Item_PutDown_01.wav", false, PLAYER_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(PLAYER_DASH1, "/Dash2.wav", false, PLAYER_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(PLAYER_DASH2, "/Dash3.wav", false, PLAYER_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(PLAYER_DASH3, "/Dash4.wav", false, PLAYER_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(PLAYER_DASH4, "/Dash5.wav", false, PLAYER_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(PLAYER_CHOP, "/KnifeChop.wav", false, PLAYER_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(PLAYER_WASHING, "/Washing1.wav", true, PLAYER_WASH_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(PLAYER_THROW, "/Throw3.wav", false, PLAYER_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(PLAYER_CATCH, "/Catch1.wav", false, PLAYER_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(PLAYER_VAN, "/VanEngine.wav", true, PLAYER_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(PLAYER_VANOUT, "/VanEngineOut.wav", false, PLAYER_CHANNEL);
    CSoundMgr::GetInstance()->
        Load_Sound(PLAYER_VANDASH, "/VanToDirt.wav", false, PLAYER_CHANNEL);
}

HRESULT CLogo::Ready_Prototype()
{    
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
    (L"Proto_RcTex", Engine::CRcTex::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
    (L"Proto_LogoTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Logo/jang.jpg", TEX_NORMAL))))
        return E_FAIL;

    //로고
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
    (L"Proto_PostCard", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Menu/Postcard.png"))))
        return E_FAIL;

    //게임 로딩 백그라운드
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
    (L"Proto_GameLoading_Back", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/GameLoading_Back.png"))))
        return E_FAIL;

    //게임 로딩
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
    (L"Proto_GameLoading", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/GameLoading%d.png", 2))))
    	return E_FAIL;

    //스테이지 번호 
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
    (L"Proto_SelectNumber", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/StageNumber%d.png", TEX_NORMAL, 6))))
        return E_FAIL;

    //스테이지 인포
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
    (L"Proto_StageInfo", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/StageInfo%d.png", TEX_NORMAL, 5))))
        return E_FAIL;

    //스테이지 인포 별점
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
    (L"Proto_StageInfo2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/StageInfo_%d.png", TEX_NORMAL, 3))))
        return E_FAIL;

    return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CLogo* pLogo = new CLogo(pGraphicDev);

    if (FAILED(pLogo->Ready_Scene()))
    {
        Safe_Release(pLogo);
        MSG_BOX("Logo Create Failed");
        return nullptr;
    }

    return pLogo;
}

void CLogo::Free()
{
    Safe_Release(m_pLoading);

    Engine::CScene::Free();
}
