#pragma once
#include "CGameObject.h"
#include "CSelectMapObject.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CPlant : public Engine::CGameObject, public CSelectMapObject
{
private:
	explicit CPlant(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlant(const CGameObject& rhs);
	virtual ~CPlant();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	void						Set_Texture(const _uint _iID);
	void						Set_Angle(const _float _fAngle);
	void						Set_Scale(const _vec3 _vScale);

private:
	HRESULT		Add_Component();
	HRESULT		Set_Metarial();


private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	_uint					m_iTextureNum;
public:
	static CPlant* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

