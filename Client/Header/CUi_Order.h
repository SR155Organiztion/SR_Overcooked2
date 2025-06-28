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
	LPDIRECT3DTEXTURE9 m_pTexFood; //���� �̹���
	LPDIRECT3DTEXTURE9 m_pTexBox; // �ֹ��� �ڽ�
	LPDIRECT3DTEXTURE9 m_pTexRecipeBox; // ��� ǥ�� �ڽ�
	LPDIRECT3DTEXTURE9 m_pTexRecipe1; //��� ������
	LPDIRECT3DTEXTURE9 m_pTexRecipe2;
	LPDIRECT3DTEXTURE9 m_pTexRecipe3;
	LPDIRECT3DTEXTURE9 m_pTexRecipe4;
	LPDIRECT3DTEXTURE9 m_pTexCook1; //���� ������
	PDIRECT3DTEXTURE9 m_pTexCook2;
	PDIRECT3DTEXTURE9 m_pTexCook3;
	float m_RecipeWidth; // �ֹ����� ���� ���� �� ���̸� ������� ��ġ/���� ���
	float m_RecipeHeight; //�ֹ����� ���� ����  �� ���̸� ������� ��� ǥ�� �ڽ� ����

public:

	void Add_Recipe(_tchar* m_pName);


};

