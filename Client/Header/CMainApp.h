#pragma once

#include "../../Base/CBase.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CManagement.h"

// namespace Engine
// {
// 	class CGraphicDev;
// 	class CManagement;
// }


class CMainApp : public CBase
{
private:// ������, �Ҹ���
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT		Ready_MainApp();
	int			Update_MainApp(const float& fTimeDelta);
	void		LateUpdate_MainApp(const float& fTimeDelta);
	void		Render_MainApp();

private:
	HRESULT		Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT		Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	Engine::CGraphicDev*	m_pDeviceClass;
	Engine::CManagement*	m_pManagementClass;
	LPDIRECT3DDEVICE9		m_pGraphicDev;



public: //�ܺ� ���� �Լ�
	static CMainApp* Create();

private: // �ܺ� �Ҹ� �Լ�
	virtual void	Free();

};

// 1. ����� �������� �̿��Ͽ� ������ Ÿ�� �÷��̾� ���¸� �����϶�.(Ű���� �Է�)

// ax + by + cz + d = 0;
// D3DXPLANE Plane;

// 2. ���콺 �浹�� �̿��Ͽ� ������ Ÿ�� �÷��̾� ���¸� �����϶�.
// D3DXIntersectTri();	//ray-picking