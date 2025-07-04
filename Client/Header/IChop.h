/**
* @file    IPlace.h
* @date    2025-07-03
* @author  �ǿ���
* @brief   ���� ���� ������Ʈ (���) �� �� �� �ִ� �������̽�
* @details �÷��̾ ���(CIngredient)�� ��� ������ ó���ϴ� ���� �������̽�.
*/
#pragma once
#include "Engine_Define.h"
class CIngredient;

class IChop 
{
public:
    virtual     ~IChop() {}

    /**
    * @brief ���� ���� �ִ� ��Ḧ ��� �����ϴ� �Լ�
    * @details ���� ���� ��ᰡ �ְ�, ��ᰡ �� �� �ִ� ���¸� ��� ����
    * @return true�� ��� ����, false�� ��� ����
    */
    virtual     _bool   Enter_Chop() = 0;

    /**
    * @brief ��� �ߴ� �Լ�
    * @details ���� ��� ������ �ߴ��ϰ� ���� ���¸� false�� ����
    */
    virtual     void    Pause_Chop() { m_bChop = false; }

    _bool       Get_Chop() const { return m_bChop; }

protected:
    /**
    * @brief ��� ���� ������Ʈ �Լ�
    * @details ��� ���� �� �ð��� ���� ���൵�� �߰�
    * @param fTimeDelta ������ �ð� (�� ����)
    */
	virtual     void    Update_Chop(const _float& fTimeDelta) = 0;

    /**
    * @brief ��� ���� ó�� �Լ�
    * @details ��� �Ϸ� �� ��� ���� ���� �� ������ �۾� ����
    */
    virtual     void    Exit_Chop() = 0;

    void        Set_Chop(_bool bChop) { m_bChop = bChop; }
    void        Set_Progress(const _float& fAdd) { m_fProgress = fAdd; }
    void        Add_Progress(const _float& fTimeDelta, const _float& fAdd) { m_fProgress += fAdd * fTimeDelta; }

    _float      Get_Progress() const { return m_fProgress; }

private:
    _bool       m_bChop = false;        ///< ���� ��� �� ����
    _float      m_fProgress = 0.f;      ///< ��� ���൵ (0.0f ~ 1.0f)
};

// class IProcess ���� �����Լ��� �����
// IProcess�� ��� �޴� IChop, ICook �����
// �ٸ����� IProcess�� ���̳��� ĳ�����ؼ� �� �� �ְ� ��������