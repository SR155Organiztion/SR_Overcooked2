/**
* @file    IProcess.h
* @date    2025-07-04
* @author  권예지
* @brief   ...
* @details ...
*/
#pragma once
#include "Engine_Define.h"
#include "CTransform.h"

class IProcess
{
public:
	virtual	_bool Enter_Process() = 0;
	virtual void Pause_Process() { m_bProcess = false; }
	virtual ~IProcess() = default;

	_bool	Get_Process() const { return m_bProcess; }
	_float	Get_Progress() const { return m_fProgress; }

protected:
	virtual void Update_Process(const _float& fTimeDelta) = 0;
	virtual void Exit_Process() = 0;

	void	Set_Process(_bool bProcess) { m_bProcess = bProcess; }

	void	Set_Progress(const _float& fProgress) { m_fProgress = fProgress; }
	void	Add_Progress(const _float& fTimeDelta, const _float& fAdd) { m_fProgress += fTimeDelta * fAdd; }

protected:
	_bool	m_bProcess = false;	///< 가공(썰기, 익히기) 진행 중 여부
	_float	m_fProgress = 0.f;	///< 가공(썰기, 익히기) 진행도 (0.0f ~ 1.0f)
};

class IChop : public IProcess
{
public:
	// IProcess을(를) 통해 상속됨
	virtual _bool Enter_Process() = 0;
	virtual void Update_Process(const _float& fTimeDelta) = 0;
	virtual void Exit_Process() = 0;
};

class ICook : public IProcess
{
public:
	// IProcess을(를) 통해 상속됨
	virtual _bool Enter_Process() = 0;
	virtual void Update_Process(const _float& fTimeDelta) = 0;
	virtual void Exit_Process() = 0;

protected:
	virtual void Update_ContentPosition(CGameObject* pTool, CGameObject* pPlacedItem)
	{
		if (pPlacedItem)
		{
			_vec3 vPos{};
			dynamic_cast<CTransform*>(pTool->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vPos);
			dynamic_cast<CTransform*>(pPlacedItem->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vPos.x, vPos.y, vPos.z - 0.1f);
		}
	}
};