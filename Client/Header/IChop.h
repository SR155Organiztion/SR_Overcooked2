/**
* @file    IPlace.h
* @date    2025-07-03
* @author  권예지
* @brief   공간 위에 오브젝트 (재료) 를 썰 수 있는 인터페이스
* @details 플레이어가 재료(CIngredient)를 써는 행위를 처리하는 공통 인터페이스.
*/
#pragma once
#include "Engine_Define.h"
class CIngredient;

class IChop 
{
public:
    virtual     ~IChop() {}

    /**
    * @brief 공간 위에 있는 재료를 썰기 시작하는 함수
    * @details 공간 위에 재료가 있고, 재료가 썰 수 있는 상태면 썰기 시작
    * @return true면 썰기 시작, false면 썰기 실패
    */
    virtual     _bool   Enter_Chop() = 0;

    /**
    * @brief 썰기 중단 함수
    * @details 현재 썰기 진행을 중단하고 내부 상태를 false로 설정
    */
    virtual     void    Pause_Chop() { m_bChop = false; }

    _bool       Get_Chop() const { return m_bChop; }

protected:
    /**
    * @brief 썰기 진행 업데이트 함수
    * @details 썰기 중일 때 시간에 따라 진행도를 추가
    * @param fTimeDelta 프레임 시간 (초 단위)
    */
	virtual     void    Update_Chop(const _float& fTimeDelta) = 0;

    /**
    * @brief 썰기 종료 처리 함수
    * @details 썰기 완료 시 재료 상태 변경 등 마무리 작업 수행
    */
    virtual     void    Exit_Chop() = 0;

    void        Set_Chop(_bool bChop) { m_bChop = bChop; }
    void        Set_Progress(const _float& fAdd) { m_fProgress = fAdd; }
    void        Add_Progress(const _float& fTimeDelta, const _float& fAdd) { m_fProgress += fAdd * fTimeDelta; }

    _float      Get_Progress() const { return m_fProgress; }

private:
    _bool       m_bChop = false;        ///< 현재 썰기 중 여부
    _float      m_fProgress = 0.f;      ///< 썰기 진행도 (0.0f ~ 1.0f)
};

// class IProcess 순수 가상함수로 만들고
// IProcess를 상속 받는 IChop, ICook 만들엇
// 다른데서 IProcess로 다이나믹 캐스팅해서 쓸 수 있게 수정하자