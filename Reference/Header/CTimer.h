#pragma once

#include "Engine_Define.h"
#include "CBase.h"

BEGIN(Engine)

class ENGINE_DLL CTimer : public CBase
{
private:
	explicit	CTimer();
	virtual		~CTimer();

public:
	_float					Get_TimeDelta() const { return m_fTimeDelta; }

public:
	HRESULT			Ready_Timer();
	void			Update_Timer();
	void			Stop_Timer();
	void			Resume_Timer();

private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_CpuTick;

	_float				m_fTimeDelta;

	_bool				m_bIsStop;

public:
	static		CTimer* Create();

private:
	virtual void	Free();
};

END
