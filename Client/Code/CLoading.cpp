#include "pch.h"
#include "CLoading.h"
#include "CProtoMgr.h"
#include "CSprite.h"
#include "Engine_Define.h"

#include "CEffectMgr.h"
#include "CTestEffect.h"


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

	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_SELECT:
		iFlag = pLoading->Loading_ForSelect();
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
	(L"Proto_Button", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Button/MainButton%d.png", 15))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_Object", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/in_game/Timer%d.png", 2))))
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


	m_bFinish = true;

	lstrcpy(m_szLoading, L"Loading Complete");

	return S_OK;
}

_uint CLoading::Loading_ForStage()
{
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlayerTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player/Player.dds", TEX_CUBE))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlayerHandTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player/PlayerHand.dds", TEX_CUBE))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_SkyBoxTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Seaweed", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_seaweed%d.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Lettuce", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_lettuce%d.png", TEX_NORMAL, 2))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Tomato", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_tomato%d.png", TEX_NORMAL, 3))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Cucumber", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_cucumber%d.png", TEX_NORMAL, 2))))
		return E_FAIL;
	
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Fish", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_fish%d.png", TEX_NORMAL, 2))))
		return E_FAIL;
	
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Shrimp", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_shrimp%d.png", TEX_NORMAL, 2))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Rice", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_rice%d.png", TEX_NORMAL, 4))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_ToolTexture_Fryingpan", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/tool/fryingpan.png", TEX_NORMAL))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_ToolTexture_Pot", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/tool/pot.png", TEX_NORMAL))))
		return E_FAIL;

	////// Plated //////
	// Plate
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlateTexture_Plate", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/plated/plated.png", TEX_NORMAL))))
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

	////// Effect //////
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_TestEffect", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", TEX_NORMAL, 90))))
		return E_FAIL;

	if (FAILED(CEffectMgr::GetInstance()->Ready_ProtoEffect
	(L"Proto_TestEffect", CTestEffect::Create(m_pGraphicDev))))
		return E_FAIL;



	////// End //////

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_IngredientTexture_Pasta", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_pasta%d.png", TEX_NORMAL, 4))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_StationBoxTexture_Ingredient", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/station/station_crate%d.dds", TEX_CUBE, 2))))
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
	(L"Proto_StationBoxTexture_Sink", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/station/station_sink_wash.dds", TEX_CUBE))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_EnvironmentTexture_Floor", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/environment/environment_floor%d.png", TEX_NORMAL, 3))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loading Complete");
	m_bFinish = true;

	return 0;
}

_uint CLoading::Loading_ForSelect()
{


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
