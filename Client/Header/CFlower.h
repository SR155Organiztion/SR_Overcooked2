#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CFlower : public Engine::CGameObject
{
private:
	explicit CFlower(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFlower(const CGameObject& rhs);
	virtual ~CFlower();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	void						Set_Texture(const _uint _iID);
	void						Set_Angle(const _float _fAngle);
	void						Set_Scale(const _vec3 _vScale);

	void						Set_Enable() { m_bEnable = true; }

private:
	HRESULT		Add_Component();
	HRESULT		Set_Metarial();


private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	//깃발 텍스쳐
	//텍스쳐 0~3은 별의 개수, 4는 아직 플레이를 하지 않음을 의미.
	_uint					m_iTextureNum;

	_bool					m_bEnable;
public:
	static CFlower* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

