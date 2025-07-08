/**
* @file		CRecipeMgr.h
* @date		2025-07-08
* @author	�ǿ���
* @brief	�丮 ������ �Ŵ��� Ŭ���� (�̱���)
* @details	�ֿ� ���
*			- ������ �ʱ�ȭ �� ���
*			- �־��� �±� �Ǵ� �̸����� ������ ��ȸ.
*			- ��� ���� �Ǻ��� ���� ������ ����
*/

#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CRecipeMgr : public CBase
{
	DECLARE_SINGLETON(CRecipeMgr)

public:
	/**
	 * @enum RECIPETYPE
	 * @brief �������� ������ �����ϴ� ������
	 */
	enum RECIPETYPE
	{
		SALAD_LETTUCE,					///< ���� ������
		SALAD_LETTUCE_TOMATO,			///< ����+�丶�� ������
		SALAD_CUCUMBER_LETTUCE_TOMATO,	///< ����+�丶��+���� ������
		SASHIMI_FISH,					///< ���� ȸ
		SASHIMI_SHRIMP,					///< ���� ȸ
		SUSHI_FISH,						///< ���� �ʹ�
		SUSHI_CUCUMBER,					///< ���� �ʹ�
		PASTA_TOMATO,					///< �丶�� �Ľ�Ÿ
		RECIPE_END						///< ��
	};

	/**
	 * @struct RECIPE
	 * @brief ���� ������ ������ ��� ����ü
	 */
	typedef struct tagRecipe
	{
		RECIPETYPE		eRecipeType;	///< ������ ����
		int				iPrice;			///< ���ǽ� �ϼ� �� ȹ�� �ݾ�
		int				iTimeLimit;		///< ���� �ð�
		set<wstring>	setIngredient;	///< ������ ��� ��� 
	}RECIPE;

private:
	explicit CRecipeMgr();
	virtual ~CRecipeMgr();

public:
	/**
	 * @brief		������ �±� ���ڿ��� ������ ������ ��ȸ�մϴ�.
	 * @param[out]	tRecipe ��ȸ�� ������ ������
	 * @param[in]	pRecipeTag ������ �±� ���ڿ�
	 * @return		���� ���� (S_OK / E_FAIL ��)
	 */
	HRESULT			Get_Recipe(RECIPE& tRecipe, const _tchar* pRecipeTag);

	/**
	 * @brief		��� ������ �����͸� �ʱ�ȭ�ϰ� ����մϴ�.
	 * @return		���� ����
	 */
	HRESULT			Ready_Recipe();

private:
	map<const _tchar*, RECIPE>	m_mapRecipe;	///< ������ �� �����̳�

public:
	virtual	void	Free();
};

END