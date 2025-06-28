#pragma once
#include "CInteract.h"
#include "ICarry.h"

class CIngredient : public CInteract, public ICarry
{
public:
	enum COOKSTATE { RAW, CHOPPED, COOKED, BURNT, DONE };

protected:
	explicit CIngredient(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CIngredient(const CGameObject& rhs);
	virtual ~CIngredient();

public:
	// ICarry을(를) 통해 상속됨
	_bool		Get_CanCarry() const override;

public:
	virtual		COOKSTATE	Get_State() const;
	virtual		void		Set_State(COOKSTATE eState);
	virtual		_bool		Is_FinalStep() const;
	virtual		void		Set_Done();

	virtual		_float		Get_Progress() const;
	virtual		void		Add_Progress(const _float& fAdd);

protected:
	virtual		void		NextState();

private:
	COOKSTATE	m_eCookState;
	_float		m_fProgress;

protected:
	virtual		void		Free();
};