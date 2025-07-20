#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CTree : public Engine::CGameObject
{
private:
	explicit CTree(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTree(const CGameObject& rhs);
	virtual ~CTree();

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

	_uint					m_iTextureNum;

	_bool					m_bEnable;
public:
	static CTree* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

