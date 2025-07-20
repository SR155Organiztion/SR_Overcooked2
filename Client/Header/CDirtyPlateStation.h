/**
* @file		CDirtyPlateStation.h
* @date		2025-07-04
* @author	�ǿ���
* @brief	������ ���� ���� (�丮 ������ ���� ������ ���� ���ƿ�)
* @details	
 */
#pragma once
#include "CInteract.h"
#include "IPlace.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CDirtyPlateStation : public CInteract, public IPlace
{
protected:
	explicit CDirtyPlateStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDirtyPlateStation(const CGameObject& rhs);
	virtual ~CDirtyPlateStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE	Get_InteractType() const override { return CInteract::STATION; }

	// IPlace��(��) ���� ��ӵ�
	_bool			Set_Place(CGameObject* pItem, CGameObject* pPlace) override { return false;  }
	_bool			Get_CanPlace(CGameObject* pItem) override { return false; }
	CGameObject*	Get_PlacedItem() override;

private:
	HRESULT			Add_Component();
	void			Set_ReturnType();
	void			Return_Plate(const _float& fTimeDelta);
	void			Update_PlatePosition();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;;
	vector<Engine::CTexture*> m_vecTextureCom;

	_bool			m_bCheck = false;
	_bool			m_bDirty = false;

	const _float	m_fInterval = 3.f;
	_float			m_fTime = 0.f;

	vector<CGameObject*>	m_vecItem;

public:
	static CDirtyPlateStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void			Free();  
};