#pragma once
#include "CGameObject.h"
#include "CUi.h"
#include "CUi_Button.h"

template<typename T>
class CUi_Factory 
{

public:
	explicit CUi_Factory(){}
	~CUi_Factory(){}

public:

	//°´Ã¼ »ý¼º
	static T* Ui_Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::UI_TYPE _cValue)
	{
		T* instance = new T(pGraphicDev);

		if (FAILED(instance->Ready_GameObject(pGraphicDev, _cValue)))
		{
			delete instance;
			return nullptr;
		}

		return instance ;
	}

	/*void Button_Create(_tchar* _m_szName);
	void Steage_create(_tchar* _m_szName);
	void Order_Create(_tchar* _m_szName);
	void Gauge_Create(_tchar* _m_szName);
	void GameEnd_Create(_tchar* _m_szName);*/

	void Render_GameObject();

	void Free();
};

