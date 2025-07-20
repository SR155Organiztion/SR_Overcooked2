#pragma once
#include "CEffect.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CAnyEffect : public CEffect
{
private:
	explicit CAnyEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnyEffect(const CAnyEffect& rhs);
	virtual ~CAnyEffect();

public:
	virtual			HRESULT		Ready_Effect() override;
	virtual			_int		Update_Effect(const _float& fTimeDelta) override;
	virtual			void		LateUpdate_Effect(const _float& fTimeDelta) override;
	virtual			void		Render_GameObject() override;

public:
	virtual			void		Play_Effect(_vec3 StartPos); // 꼭 오버라이딩

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	const _tchar*		m_szTextureKey{};	///< 등록된 텍스쳐 키값
	_int				m_iCount{};			///< 텍스쳐의 갯수
	_vec3				m_vEffectPos{};		///< Effect 출력할 위치 (출력될 오브젝트 기준)
	_vec3				m_vEffectScale{};	///< Effect 스케일
	_float				m_fAccel{};			///< 이펙트 스프라이트 전환 가속


	_float				m_fFrame{};

public:
	virtual			CEffect* Clone(); // 꼭 오버라이딩 할 것 
	/**
	* @brief ProtoEffect 생성 함수
	* @param ProtoTextureName - 등록해둔 텍스쳐의 키 값
	* - Count - 텍스쳐의 갯수
	* - fAccel - 텍스쳐 전환 속도. (default = 1.f)
	* - vRenderPos - 텍스쳐를 렌더링할 오브젝트 기준 위치(default = {0,0,0})
	*/
	static			CEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
		const _tchar* ProtoTextureName, _int Count, _float fAccel, _vec3 vRenderPos, _vec3 vScale);

private:
	virtual			void		Free() override;

};

