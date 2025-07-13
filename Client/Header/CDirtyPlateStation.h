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
	_bool			Get_CanPlace(CGameObject* pItem) override;

private:
	HRESULT			Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;;
	vector<Engine::CTexture*> m_vecTextureCom;

public:
	static CDirtyPlateStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void			Free(); 
};