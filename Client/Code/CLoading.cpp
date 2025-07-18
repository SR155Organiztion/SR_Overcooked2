#include "pch.h"
#include "CLoading.h"
#include "CProtoMgr.h"
#include "CSprite.h"
#include "CVertexShader.h"
#include "CShader.h"
#include "Engine_Define.h"
#include "CUi_Factory.h"
#include "CEffectMgr.h"
#include "CTestEffect.h"
#include "CFireEffect.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), m_bFinish(false)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_szLoading, sizeof(m_szLoading));
}

CLoading::~CLoading(void)
{
}

unsigned int __stdcall CLoading::Thread_Main(void* pArg)
{
	CLoading* pLoading = reinterpret_cast<CLoading*>(pArg);

	_uint		iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_LOGO:
		iFlag = pLoading->Loading_ForLogo();
		break;

	case LOADING_SELECT:
		iFlag = pLoading->Loading_ForSelect();
		break;

	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	// _endthreadex(0);

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_LoadingID = eID;

	return S_OK;
}

_uint CLoading::Loading_ForLogo()
{
	lstrcpy(m_szLoading, L"Buffer Component Loading...........................");

	/*if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_RcTex", Engine::CRcTex::Create(m_pGraphicDev))))
		return E_FAIL;*/

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_TriCol", Engine::CTriCol::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_RcCol", Engine::CRcCol::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_TerrainTex", Engine::CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_CubeTex", Engine::CCubeTex::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_RcTileTex", Engine::CRcTileTex::Create(m_pGraphicDev))))
		return E_FAIL;
	

	lstrcpy(m_szLoading, L"Etc Component Loading...........................");

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_Calculator", Engine::CCalculator::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_FSM", Engine::CFSMComponent::Create(m_pGraphicDev))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Sprite Component Loading...........................");

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlayerTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player/Player%d.dds", TEX_CUBE, 3))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlayerHandTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player/PlayerHand.dds", TEX_CUBE))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_Button", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Button/MainButton%d.png", 15))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_Timer", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/Timer%d.png", 2))))
		return E_FAIL;
	
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_Score", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/Score%d.png", 2))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_Coin", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/Coin%d.png", 42))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_ObjectBox", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/Recipe_%d.png", 8))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_ObjectGauge", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/Recipe_Gauge%d.png", 2))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_ObjectImage", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/Recipe%d.png", 8))))
		return E_FAIL;

	//재료 아이콘
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_Icon2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/Icon%d.png", TEX_NORMAL, 9))))
		return E_FAIL;

	//요리 만들 때 로딩창 
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_Cook", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/Cook_Loding%d.png", TEX_NORMAL, 2))))
		return E_FAIL;

	//경고창 
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_Warning", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/Warning0.png", TEX_NORMAL))))
		return E_FAIL;

	//레디 고 타임아웃
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_TimeOut", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/TimeOut%d.png", 3))))
		return E_FAIL;

	//총점 정리 별
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_StarScore", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/Complete_Score%d.png", 4))))
		return E_FAIL;

	//총점 정리 배경 
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_Complete", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/Complete%d.png", 2))))
		return E_FAIL;

	//페이드아웃
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_Fadeout", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/Fadeout%d.png", 4))))
		return E_FAIL;

	//스테이지 번호 
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_SelectNumber", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/StageNumber%d.png", TEX_NORMAL, 6))))
		return E_FAIL;

	m_bFinish = true;

	lstrcpy(m_szLoading, L"Loading Complete");

	return S_OK;
}

_uint CLoading::Loading_ForStage()
{
	lstrcpy(m_szLoading, L"Loading Start");
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_SkyBoxTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_DummyEffect", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", TEX_NORMAL, 90))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_CloudEffect", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Effect/cloud%d.png", TEX_NORMAL, 9))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_FireEffect", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Effect/Fire/fire%d.png", TEX_NORMAL, 32))))
		return E_FAIL;

	////// Ingredients //////
	// Seaweed
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Seaweed", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_seaweed%d.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Seaweed_Alpha", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_seaweed%d_alpha.png", TEX_NORMAL))))
		return E_FAIL;

	// Lettuce
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Lettuce", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_lettuce%d.png", TEX_NORMAL, 2))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Lettuce_Alpha", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_lettuce%d_alpha.png", TEX_NORMAL, 2))))
		return E_FAIL;

	// Tomato
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Tomato", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_tomato%d.png", TEX_NORMAL, 5))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Tomato_Alpha", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_tomato%d_alpha.png", TEX_NORMAL, 5))))
		return E_FAIL;

	// Cucumber
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Cucumber", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_cucumber%d.png", TEX_NORMAL, 2))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Cucumber_Alpha", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_cucumber%d_alpha.png", TEX_NORMAL, 2))))
		return E_FAIL;
	
	// Fish
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Fish", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_fish%d.png", TEX_NORMAL, 2))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Fish_Alpha", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_fish%d_alpha.png", TEX_NORMAL, 2))))
		return E_FAIL;
	
	// Shrimp
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Shrimp", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_shrimp%d.png", TEX_NORMAL, 2))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Shrimp_Alpha", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_shrimp%d_alpha.png", TEX_NORMAL, 2))))
		return E_FAIL;

	// Rice
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Rice", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_rice%d.png", TEX_NORMAL, 4))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Rice_Alpha", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_rice%d_alpha.png", TEX_NORMAL, 4))))
		return E_FAIL;

	// Pasta
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Pasta", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_pasta%d.png", TEX_NORMAL, 4))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Pasta_Alpha", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_pasta%d_alpha.png", TEX_NORMAL, 4))))
		return E_FAIL;

	////// Tool //////
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_ToolTexture_Fryingpan", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/tool/fryingpan.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_ToolTexture_Fryingpan_Alpha", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/tool/fryingpan_alpha.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_ToolTexture_Pot", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/tool/pot.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_ToolTexture_Pot_Alpha", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/tool/pot_alpha.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_ToolTexture_FireExtinguisher", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/tool/fireextinguisher.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_ToolTexture_FireExtinguisher_Alpha", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/tool/fireextinguisher_alpha.png", TEX_NORMAL))))
		return E_FAIL;

	////// Plated //////
	// Plate
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_Alpha", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_alpha.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_dirty", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_dirty.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_wrong", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_wrong.png", TEX_NORMAL))))
		return E_FAIL;

	// Salad
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_cucumber", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_cucumber.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_cucumber_lettuce", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_cucumber_lettuce.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_cucumber_tomato", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_cucumber_tomato.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_cucumber_lettuce_tomato", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_cucumber_lettuce_tomato.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_lettuce", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_lettuce.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_lettuce_tomato", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_lettuce_tomato.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_tomato", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_tomato.png", TEX_NORMAL))))
		return E_FAIL;

	// sashimi
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_fish", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_fish.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_shrimp", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_prawn.png", TEX_NORMAL))))
		return E_FAIL;

	// Sushi
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_rice", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_rice.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_fish_rice", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_fish_rice.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_fish_rice_seaweed", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_fish_rice_seaweed.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_fish_seaweed", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_fish_seaweed.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_cucumber_rice", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_cucumber_rice.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_rice_seaweed", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_rice_seaweed.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_seaweed", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_seaweed.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_cucumber_seaweed", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_cucumber_seaweed.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_cucumber_rice_seaweed", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_cucumber_rice_seaweed.png", TEX_NORMAL))))
		return E_FAIL;

	// Pasta
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_pasta", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_pasta.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_pasta_tomatosoup", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_pasta_tomatosoup.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate_tomatosoup", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated_tomatosoup.png", TEX_NORMAL))))
		return E_FAIL;

	// NPC
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_OnionKing", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/OnionKing%d.png", TEX_NORMAL, 12))))
		return E_FAIL;

	////// End //////




	////// Station //////
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_StationBoxTexture_Alpha", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/station/station_alpha.dds", TEX_CUBE))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_StationBoxTexture_Ingredient", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/station/station_crate%d.dds", TEX_CUBE, 2))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_StationLidTexture_Ingredient", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/station/station_crate_lid%d.png", TEX_NORMAL, 8))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_StationBoxTexture_Chop", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/station/station_chop.dds", TEX_CUBE))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_StationBoxTexture_Gas", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/station/station_gas%d.dds", TEX_CUBE, 2))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_StationBoxTexture_Empty", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/station/station_empty.dds", TEX_CUBE))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_StationBoxTexture_Sink", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/station/station_sink_wash%d.dds", TEX_CUBE, 2))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_StationBoxTexture_CleanPlate", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/station/station_cleanplate.dds", TEX_CUBE))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_StationBoxTexture_DirtyPlate", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/station/station_dirtyplate.dds", TEX_CUBE))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_StationBoxTexture_Serving", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/station/station_serving.dds", TEX_CUBE))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_StationBoxTexture_Trash", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/station/station_trash.dds", TEX_CUBE))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_StationBoxTexture_Invisible", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/station/station_invisible.dds", TEX_CUBE))))
		return E_FAIL;

	////// Environment Tile //////
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_EnvironmentTexture_Tile_Blue33", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Tile/Tile_Blue33.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_EnvironmentTexture_Tile_Blue44", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Tile/Tile_Blue44.png", TEX_NORMAL))))
		return E_FAIL;
  
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_EnvironmentTexture_Tile_Pink44", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Tile/Tile_Pink44.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_EnvironmentTexture_Tile_StoneBrown", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Tile/Tile_StoneBrown.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_EnvironmentTexture_Tile_StoneBeige", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Tile/Tile_StoneBeige.png", TEX_NORMAL))))
		return E_FAIL;

	////// Environment Stage Object //////

  	////// Effect //////
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_TestEffect", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", TEX_NORMAL, 90))))
		return E_FAIL;

	if (FAILED(CEffectMgr::GetInstance()->Ready_ProtoEffect
	(L"Proto_TestEffect", CTestEffect::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(CEffectMgr::GetInstance()->Ready_ProtoEffect
	(L"Proto_FireEffect", CFireEffect::Create(m_pGraphicDev))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loading Complete");
	m_bFinish = true;

	return 0;
}

_uint CLoading::Loading_ForSelect()
{
	lstrcpy(m_szLoading, L"Loading Start");
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_2DShader", CVertexShader::Create(m_pGraphicDev, CShader::CUBE_DECL))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_HexTileTex", CHexTileTex::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_HexPrismTex", CHexPrismTex::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_HexTileTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Tile/HexTile_%d.png", TEX_NORMAL, 2))))
		return E_FAIL;

	////// Environment Map Object //////`
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_EnvironmentObject_Map_Flag", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Environment/Map/Flag_%d.png", TEX_NORMAL, 5))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_EnvironmentObject_Map_Tree", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Environment/Map/Tree_%d.png", TEX_NORMAL, 4))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_EnvironmentObject_Map_Flower", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Environment/Map/Flower_%d.png", TEX_NORMAL, 2))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_EnvironmentObject_Map_Plant", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Environment/Map/Plant_%d.png", TEX_NORMAL, 2))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_EnvironmentObject_Map_Castle", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Environment/Map/Castle.png", TEX_NORMAL))))
		return E_FAIL;

	m_bFinish = true;

	lstrcpy(m_szLoading, L"Loading Complete");
	return 0;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading* pLoading = new CLoading(pGraphicDev);

	if (FAILED(pLoading->Ready_Loading(eID)))
	{
		Safe_Release(pLoading);
		MSG_BOX("CLoading Create Failed");
		return nullptr;
	}

	return pLoading;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}
