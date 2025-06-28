#pragma once
#include "CUi_Object.h"
#include "CTransform.h"

class CUi_Order :  public CUi_Object
{

protected:
	CUi_Order(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Order(const CGameObject& rhs);
	~CUi_Order();

private: 
	Engine::CTransform* m_pTransformCom;
	LPDIRECT3DTEXTURE9 m_pTexFood; //음식 이미지
	LPDIRECT3DTEXTURE9 m_pTexBox; // 주문서 박스
	LPDIRECT3DTEXTURE9 m_pTexRecipeBox; // 재료 표시 박스
	LPDIRECT3DTEXTURE9 m_pTexRecipe1; //재료 아이콘
	LPDIRECT3DTEXTURE9 m_pTexRecipe2;
	LPDIRECT3DTEXTURE9 m_pTexRecipe3;
	LPDIRECT3DTEXTURE9 m_pTexRecipe4;
	LPDIRECT3DTEXTURE9 m_pTexCook1; //조리 아이콘
	PDIRECT3DTEXTURE9 m_pTexCook2;
	PDIRECT3DTEXTURE9 m_pTexCook3;
	float m_RecipeWidth; // 주문서의 가로 길이 ← 길이를 기반으로 위치/간격 계산
	float m_RecipeHeight; //주문서의 세로 길이  ← 길이를 기반으로 재료 표시 박스 출현

public:

	void Add_Recipe(_tchar* m_pName);


};

