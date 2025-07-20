/**
 * @file    CObjectPoolMgr.h
 * @date    2025-07-09
 * @author  �ǿ���
 * @brief   ���� ������Ʈ Ǯ�� �����ϴ� �̱��� Ŭ����
 * @details	�� Ŭ������ ���� ���� �� �����Ǵ� ���� ������Ʈ���� �̸� ������ �����ϴ� ������Ʈ Ǯ�� �ý����� ����
 *          - ������Ʈ ��� �� ��ȯ
 *          - ������Ʈ ��ȸ
 */
#pragma once
#include "Engine_Define.h"
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CObjectPoolMgr : public CBase
{
	DECLARE_SINGLETON(CObjectPoolMgr)

private:
	explicit CObjectPoolMgr();
	virtual ~CObjectPoolMgr();

public:
	/**
	 * @brief		������Ʈ Ǯ�� ������Ʈ�� ���
	 * @param[in]	pObjTag - ������Ʈ �±� (Ű)
	 * @param[in]	pGameObject - ����� ���� ������Ʈ ������
	 * @return		S_OK ����, E_FAIL ����
	 */
	HRESULT			Register_Object(const _tchar* pObjTag, CGameObject* pGameObject);

	/**
	 * @brief		������Ʈ Ǯ���� ������Ʈ �ϳ��� ����
	 * @param[in]	pObjTag - ������Ʈ �±�
	 * @return		��� ������ ������Ʈ ������, ���� ��� nullptr
	 */
	CGameObject*	Get_Object(const _tchar* pObjTag);

	CGameObject*	Get_Object(const _tchar* pObjTag, const _vec3 vPos);

	/**
	 * @brief		����� ���� ������Ʈ�� Ǯ�� ��ȯ
	 * @param[in]	pObjTag - ������Ʈ �±�
	 * @param[in]	pGameObject - ��ȯ�� ���� ������Ʈ ������
	 */
	void			Return_Object(const _tchar* pObjTag, CGameObject* pGameObject);

	_bool			Is_Empty(const _tchar* pObjTag);

private:
	/**
	 * @brief		������Ʈ �±׺� ������Ʈ ����Ʈ ��
	 * @details		���� �±׷� ��ϵ� ������Ʈ���� ���Ϳ� ����Ǿ� ����
	 */
	unordered_map<const _tchar*, vector<CGameObject*>>	m_mapObject;
public:
	virtual	void	Free();
};

END