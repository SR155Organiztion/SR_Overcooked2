#pragma once
#include "CInteract.h"
#include "IPlace.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CDishStation : public CInteract, public IPlace
{
protected:
	explicit CDishStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDishStation(const CGameObject& rhs);
	virtual ~CDishStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	// IPlace��(��) ���� ��ӵ�
	_bool Get_CanPlace(ICarry* pCarry) const override;
	void Set_CarryTypes() override;

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CDishStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};