#include "CRecipeMgr.h"

IMPLEMENT_SINGLETON(CRecipeMgr)

CRecipeMgr::CRecipeMgr()
{
}

CRecipeMgr::~CRecipeMgr()
{
	Free();
}

HRESULT CRecipeMgr::Get_Recipe(RECIPE& tRecipe, const _tchar* pRecipeTag)
{
	auto iter = find_if(m_mapRecipe.begin(), m_mapRecipe.end(), CTag_Finder(pRecipeTag));

	if (iter == m_mapRecipe.end())
		return E_FAIL;

	tRecipe = iter->second;
	return S_OK;
} 

HRESULT CRecipeMgr::Ready_Recipe()
{
	RECIPE tRecipe{};
	 
	// 1) salad_lettuce
	tRecipe.eRecipeType = SALAD_LETTUCE;
	tRecipe.iPrice = 20;
	tRecipe.iTimeLimit = 75;
	tRecipe.setIngredient.clear();
	tRecipe.setIngredient.insert(L"lettuce");
	m_mapRecipe.insert({L"salad_lettuce", tRecipe});

	// 2) salad_lettuce_tomato
	tRecipe.eRecipeType = SALAD_LETTUCE_TOMATO;
	tRecipe.iPrice = 40;
	tRecipe.iTimeLimit = 80;
	tRecipe.setIngredient.clear();
	tRecipe.setIngredient.insert(L"lettuce");
	tRecipe.setIngredient.insert(L"tomato");
	m_mapRecipe.insert({ L"salad_lettuce_tomato", tRecipe });

	// 3) salad_cucumber_lettuce_tomato
	tRecipe.eRecipeType = SALAD_CUCUMBER_LETTUCE_TOMATO;
	tRecipe.iPrice = 60;
	tRecipe.iTimeLimit = 85;
	tRecipe.setIngredient.clear();
	tRecipe.setIngredient.insert(L"lettuce");
	tRecipe.setIngredient.insert(L"tomato");
	tRecipe.setIngredient.insert(L"cucumber");
	m_mapRecipe.insert({ L"salad_cucumber_lettuce_tomato", tRecipe });

	// 4) sashimi_fish
	tRecipe.eRecipeType = SASHIMI_FISH;
	tRecipe.iPrice = 20;
	tRecipe.iTimeLimit = 75;
	tRecipe.setIngredient.clear();
	tRecipe.setIngredient.insert(L"fish");
	m_mapRecipe.insert({ L"sashimi_fish", tRecipe });

	// 5) sashimi_shrimp
	tRecipe.eRecipeType = SASHIMI_SHRIMP;
	tRecipe.iPrice = 20;
	tRecipe.iTimeLimit = 75;
	tRecipe.setIngredient.clear();
	tRecipe.setIngredient.insert(L"shrimp");
	m_mapRecipe.insert({ L"sashimi_shrimp", tRecipe });

	// 6) sushi_fish
	tRecipe.eRecipeType = SUSHI_FISH;
	tRecipe.iPrice = 60;
	tRecipe.iTimeLimit = 80;
	tRecipe.setIngredient.clear();
	tRecipe.setIngredient.insert(L"seaweed");
	tRecipe.setIngredient.insert(L"rice");
	tRecipe.setIngredient.insert(L"fish");
	m_mapRecipe.insert({ L"sushi_fish", tRecipe });

	// 7) sushi_cucumber
	tRecipe.eRecipeType = SUSHI_CUCUMBER;
	tRecipe.iPrice = 60;
	tRecipe.iTimeLimit = 80;
	tRecipe.setIngredient.clear();
	tRecipe.setIngredient.insert(L"seaweed");
	tRecipe.setIngredient.insert(L"rice");
	tRecipe.setIngredient.insert(L"cucumber");
	m_mapRecipe.insert({ L"sushi_cucumber", tRecipe });

	// 8) pasta_tomato
	tRecipe.eRecipeType = PASTA_TOMATO;
	tRecipe.iPrice = 40;
	tRecipe.iTimeLimit = 90;
	tRecipe.setIngredient.clear();
	tRecipe.setIngredient.insert(L"pasta");
	tRecipe.setIngredient.insert(L"tomatosoup");
	m_mapRecipe.insert({ L"pasta_tomato", tRecipe });

	return S_OK;
}

void CRecipeMgr::Free()
{
	m_mapRecipe.clear();
}