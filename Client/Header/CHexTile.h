#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CHexTileTex;
	class CHexPrismTex;
	class CTransform;
	class CTexture;
}

class CHexTile : public Engine::CGameObject
{
private:
	explicit CHexTile(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHexTile(const CGameObject& rhs);
	virtual ~CHexTile();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	void						Set_TextureNum(_uint _iID);
	void						Flip(const _float& fTimeDelta);
private:
	HRESULT		Add_Component();
	HRESULT		Set_Metarial();


private:
	Engine::CHexTileTex* m_pTileBufferCom;
	Engine::CHexPrismTex* m_pPrismBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	
	_uint					m_iTextureNum;

	_bool					m_bFliped;
	_bool					m_bAction[3];
	_float					m_fHeight;
public:
	static CHexTile* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

