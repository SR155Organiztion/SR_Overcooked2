#pragma once
#include "CGameObject.h"
#include "CUi.h"
#include "CUi_Button.h"
#include "CUi_Icon.h"

template<typename T>
class CUi_Factory 
{

CUi_Icon* CUi_Factory::m_pProtoIcon = nullptr;

public:


	explicit CUi_Factory(){}
	~CUi_Factory(){}

public:

	//°´Ã¼ »ý¼º
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

	static T* Ui_Create(const _tchar* _Name)
	{
		CUi_Icon* pProtoIcon = new CUi_Icon(m_pGraphicDev);
		pProtoIcon->Ready_GameObject(m_pGraphicDev);
		CProtoMgr::GetInstance()->Ready_Prototype(_Name, pProtoIcon);

		CUi_Icon* pCloneIcon = dynamic_cast<CUi_Icon*>(CProtoMgr::GetInstance()->Clone_Prototype(_Name));
	}

	static void InitPrototype(LPDIRECT3DDEVICE9 pDevice)
	{
		if (!m_pProtoIcon)  
		{
			m_pProtoIcon = new CUi_Icon(pDevice);      
			m_pProtoIcon->Ready_GameObject(pDevice);        
		}
	}

	static CUi_Icon* GetProtoIcon(LPDIRECT3DDEVICE9 dev)
	{
		static CUi_Icon protoIcon(dev);
		static bool inited = false;
		if (!inited)
		{
			protoIcon.Ready_GameObject(dev);
			inited = true;
		}
		return &protoIcon;
	}

	static CUi_Icon* CreateIcon(LPDIRECT3DDEVICE9 dev)
	{
		return new CUi_Icon(*GetProtoIcon(dev));
	}
};

void Render_GameObject() {}

void Free() {}

};

