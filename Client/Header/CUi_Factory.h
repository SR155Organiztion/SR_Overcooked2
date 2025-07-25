#pragma once
#include "CGameObject.h"
#include "CUi.h"
#include "CUi_Button.h"
#include "CUi_Icon.h"

template<typename T>
class CUi_Factory 
{
public:
	explicit CUi_Factory(){}
	~CUi_Factory(){}

public:

	//��ü ����
	static T* Ui_Create(LPDIRECT3DDEVICE9 pGraphicDev, BUTTON_TYPE _cValue)
	{
		T* instance = new T(pGraphicDev);

		if (FAILED(instance->Ready_GameObject(pGraphicDev, _cValue)))
		{
			delete instance;
			return nullptr;
		}

		return instance;
	}

	static T* Ui_Create(LPDIRECT3DDEVICE9 pGraphicDev, GAUGE_TYPE _cValue)
	{
		T* instance = new T(pGraphicDev);

		if (FAILED(instance->Ready_GameObject(pGraphicDev, _cValue)))
		{
			delete instance;
			return nullptr;
		}

		return instance ;
	}

	static T* Ui_Create(LPDIRECT3DDEVICE9 m_pGraphicDev)
	{
		T* instance = new T(m_pGraphicDev);

		if (FAILED(instance->Ready_GameObject(m_pGraphicDev)))
		{
			delete instance;
			return nullptr;
		}

		return instance;
	}

	

void Render_GameObject() {}

void Free() {}

};

