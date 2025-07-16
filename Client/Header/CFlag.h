#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CFlag : public Engine::CGameObject
{
private:
	explicit CFlag(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFlag(const CGameObject& rhs);
	virtual ~CFlag();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	void						Set_TextureNum(_uint _iID);
	void						Set_Angle(_float _fAngle);
private:
	HRESULT		Add_Component();
	HRESULT		Set_Metarial();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	_uint					m_iTextureNum;
public:
	static CFlag* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

