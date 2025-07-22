#pragma once
#include "CInteract.h"
#include "CIngredient.h"

namespace Engine
{
	class CCubeTex;
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CDispenserStation : public CInteract
{
protected:
	explicit CDispenserStation(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDispenserStation();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE			Get_InteractType() const override { return CInteract::STATION; }

private:
	HRESULT		Add_Component();
	void		Throw_Ingredient();
	void		Check_ThrowCool(const _float& dt);
	void		Check_FirstPause(const _float& dt);


private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	CIngredient::INGREDIENT_TYPE	m_eTypeIngredient;		///< station�� � Ÿ���� ��Ḧ �����ϴ��� ���п�
	const _tchar* m_szIngredientName;						///< ObjectPoolMgr ȣ��� ���ڿ�

	_bool	m_bThrow{}, m_bFirstPause{};
	_float	m_fThrowCool{}, m_fFirstPause{};



private: //Test
	_bool	m_bTest = true;
	_float	m_fTest{};
	_int	m_iTest{};

public:
	static CDispenserStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	// create�� ��������� ����
	void	Set_TypeIngredientStation(const _tchar* create_name);

private:
	virtual		void		Free();

};

