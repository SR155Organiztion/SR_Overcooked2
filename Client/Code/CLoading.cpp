#include "pch.h"
#include "CLoading.h"
#include "CProtoMgr.h"
#include "CSprite.h"
#include "Engine_Define.h"

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

	case LOADING_STAGE:
		iFlag = pLoading->Loaing_ForStage();
		break;

	case LOADING_BOSS:
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

_uint CLoading::Loaing_ForStage()
{
	lstrcpy(m_szLoading, L"Buffer Component Loading...........................");


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

	lstrcpy(m_szLoading, L"Texture Component Loading...........................");

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlayerTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player/Player.dds", TEX_CUBE))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_PlayerHandTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player/PlayerHand.dds", TEX_CUBE))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_TerrainTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", TEX_NORMAL, 2))))
		return E_FAIL;


	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_SkyBoxTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_Effect", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", TEX_NORMAL, 90))))
		return E_FAIL;


	lstrcpy(m_szLoading, L"Sprite Component Loading...........................");

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_Ui", Engine::CSprite::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/t_ui_main_menu_button_01_d.png", SPRITE_BUTTON))))
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
	(L"Proto_IngredientTexture_Pasta", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/ingredient/ingredient_pasta%d.png", TEX_NORMAL, 4))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_StationBoxTexture_EmptyStation", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/station/station_empty.dds", TEX_CUBE))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_EnvironmentTexture_Floor", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Object/environment/environment_floor%d.png", TEX_NORMAL, 3))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Etc Component Loading...........................");

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev))))
		return E_FAIL;

	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype
	(L"Proto_Calculator", Engine::CCalculator::Create(m_pGraphicDev))))
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
