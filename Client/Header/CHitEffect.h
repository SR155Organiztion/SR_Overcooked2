#pragma once
#include "CEffect.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CHitEffect : public CEffect
{
private:
	explicit CHitEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHitEffect(const CHitEffect& rhs);
	virtual ~CHitEffect();

public:
	virtual			HRESULT		Ready_Effect() override;
	virtual			_int		Update_Effect(const _float& fTimeDelta) override;
	virtual			void		LateUpdate_Effect(const _float& fTimeDelta) override;
	virtual			void		Render_GameObject() override;

public:
	virtual			void		Play_Effect(_vec3 StartPos); // 꼭 오버라이딩

private:
	HRESULT			Add_Component();
	void			Set_RandomPos(const _float& dt);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	_float				m_fFrame{};
	_vec3				m_vOwnerPos{};
	_bool				m_bSetPos{};

public:
	virtual			CEffect* Clone(); // 꼭 오버라이딩 할 것 
	/**
	* @brief ProtoEffect 생성 함수
	* @param ProtoTextureName - 등록해둔 텍스쳐의 키 값
	* - Count - 텍스쳐의 갯수
	* - fAccel - 텍스쳐 전환 속도. (default = 1.f)
	* - vRenderPos - 텍스쳐를 렌더링할 오브젝트 기준 위치(default = {0,0,0})
	*/
	static			CEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual			void		Free() override;

};