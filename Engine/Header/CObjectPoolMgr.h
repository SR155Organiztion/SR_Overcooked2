/**
 * @file    CObjectPoolMgr.h
 * @date    2025-07-09
 * @author  권예지
 * @brief   게임 오브젝트 풀을 관리하는 싱글톤 클래스
 * @details	이 클래스는 자주 생성 및 삭제되는 게임 오브젝트들을 미리 생성해 재사용하는 오브젝트 풀링 시스템을 관리
 *          - 오브젝트 등록 및 반환
 *          - 오브젝트 조회
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
	 * @brief		오브젝트 풀에 오브젝트를 등록
	 * @param[in]	pObjTag - 오브젝트 태그 (키)
	 * @param[in]	pGameObject - 등록할 게임 오브젝트 포인터
	 * @return		S_OK 성공, E_FAIL 실패
	 */
	HRESULT			Register_Object(const _tchar* pObjTag, CGameObject* pGameObject);

	/**
	 * @brief		오브젝트 풀에서 오브젝트 하나를 꺼냄
	 * @param[in]	pObjTag - 오브젝트 태그
	 * @return		사용 가능한 오브젝트 포인터, 없을 경우 nullptr
	 */
	CGameObject*	Get_Object(const _tchar* pObjTag);

	CGameObject*	Get_Object(const _tchar* pObjTag, const _vec3 vPos);

	/**
	 * @brief		사용이 끝난 오브젝트를 풀에 반환
	 * @param[in]	pObjTag - 오브젝트 태그
	 * @param[in]	pGameObject - 반환할 게임 오브젝트 포인터
	 */
	void			Return_Object(const _tchar* pObjTag, CGameObject* pGameObject);

	_bool			Is_Empty(const _tchar* pObjTag);

private:
	/**
	 * @brief		오브젝트 태그별 오브젝트 리스트 맵
	 * @details		같은 태그로 등록된 오브젝트들이 벡터에 저장되어 재사용
	 */
	unordered_map<const _tchar*, vector<CGameObject*>>	m_mapObject;
public:
	virtual	void	Free();
};

END