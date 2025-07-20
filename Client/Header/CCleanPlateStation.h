/**
* @file		CCleanPlateStation.h
* @date		2025-07-02
* @author	�ǿ���
* @brief	������ ���� ���� (�Ƹ��� ��ũ�뿡�� ��ô��)
* @details	(���� ������ ���� ����)
 *			- �÷��̾ ����(������ ���� ��)�� ������ �� �ִ� ������ �����ϴ� �����̼� Ŭ�����Դϴ�.
 *			- ���� ���� �� ���� ���� �� ���ð� �� �����̼ǿ� �غ�Ǿ� ���� �� �ֽ��ϴ�. (���� Ȯ�� �ʿ�)
 *			- �÷��̾ ������ ��������, ������ ��(ä��) ���� �� ���� ������ ���ð� �� �����̼� ���� �ö�ɴϴ�.
 *			- �÷��̾�� � ������ ��� �ֵ��� ����, �� �����̼ǿ� ������ ���� �������� �� �����ϴ�.
 *			- ����, IPlace �������̽��� Get_CanPlace() �Լ����� �׻� false�� ��ȯ�Ͽ�, �÷��̾ ���� �ø��� ������ �����մϴ�.
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

class CCleanPlateStation : public CInteract, public IPlace
{
protected:
	explicit CCleanPlateStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCleanPlateStation(const CGameObject& rhs);
	virtual ~CCleanPlateStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE	Get_InteractType() const override { return CInteract::STATION; }

	// IPlace��(��) ���� ��ӵ�
	_bool			Set_Place(CGameObject* pItem, CGameObject* pPlace);
	_bool			Get_CanPlace(CGameObject* pItem) override;
	CGameObject*	Get_PlacedItem();

private:
	HRESULT			Add_Component();
	void			Update_PlatePosition();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	vector<Engine::CTexture*> m_vecTextureCom;

	vector<CGameObject*>	m_vecItem;

public:
	static CCleanPlateStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void			Free(); 
};