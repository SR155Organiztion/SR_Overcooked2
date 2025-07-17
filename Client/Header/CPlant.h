#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CPlant : public Engine::CGameObject
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

	void						Set_Texture(_uint _iID);
	void						Set_Angle(_float _fAngle);

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

public:
	static CPlant* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

