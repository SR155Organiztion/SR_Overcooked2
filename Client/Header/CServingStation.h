/**
* @file    CServingStation.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ���� �����̼�(Serving Station) ������Ʈ Ŭ����
* @details �÷��̾ �ϼ��� �丮�� �������ų� ������ �� �ִ� ������ ����.
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

class CServingStation : public CInteract, public IPlace
{
protected:
	explicit CServingStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CServingStation(const CGameObject& rhs);
	virtual ~CServingStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	// IPlace��(��) ���� ��ӵ�
	/**
	* @brief �ش� ������ �������� �������� �� �ִ��� Ȯ���ϴ� �Լ�.
	* @param pCarry - �������� ICarry ������.
	* @return true�� �������� ����, false�� �Ұ���.
	*/
	_bool Get_CanPlace(ICarry* pCarry) const override;
	/**
	* @brief �ش� ������ ���� �� �ִ� ������ Ÿ���� �����ϴ� �Լ�.
	* @details ��� Ŭ�������� m_setCarryTypes�� ����� ICarry Ÿ���� ���� �߰�.
	*/
	void Set_CarryTypes() override;

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CServingStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};