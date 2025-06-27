#ifndef InputDev_h__
#define InputDev_h__

#include "Engine_Define.h"
#include "CBase.h"

BEGIN(Engine)

class ENGINE_DLL CDInputMgr : public CBase
{
	DECLARE_SINGLETON(CDInputMgr)

private:
	explicit CDInputMgr(void);
	virtual ~CDInputMgr(void);

public:
	_byte	Get_DIKeyState(_ubyte byKeyID)
	{
		return m_byKeyState[byKeyID];
	}

	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)
	{
		return m_tMouseState.rgbButtons[eMouse];
	}

	// ���� ���콺�� Ư�� �� ��ǥ�� ��ȯ
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{
		return *(((_long*)&m_tMouseState) + eMouseState);
	}

public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Update_InputDev(void);

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

private:
	_byte					m_byKeyState[256];		// Ű���忡 �ִ� ��� Ű���� �����ϱ� ���� ����
	DIMOUSESTATE			m_tMouseState;

public:
	virtual void	Free(void);

};
END
#endif // InputDev_h__


