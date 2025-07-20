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
	virtual			void		Play_Effect(_vec3 StartPos); // �� �������̵�

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	const _tchar*		m_szTextureKey{};	///< ��ϵ� �ؽ��� Ű��
	_int				m_iCount{};			///< �ؽ����� ����
	_vec3				m_vEffectPos{};		///< Effect ����� ��ġ (��µ� ������Ʈ ����)
	_vec3				m_vEffectScale{};	///< Effect ������
	_float				m_fAccel{};			///< ����Ʈ ��������Ʈ ��ȯ ����


	_float				m_fFrame{};

public:
	virtual			CEffect* Clone(); // �� �������̵� �� �� 
	/**
	* @brief ProtoEffect ���� �Լ�
	* @param ProtoTextureName - ����ص� �ؽ����� Ű ��
	* - Count - �ؽ����� ����
	* - fAccel - �ؽ��� ��ȯ �ӵ�. (default = 1.f)
	* - vRenderPos - �ؽ��ĸ� �������� ������Ʈ ���� ��ġ(default = {0,0,0})
	*/
	static			CEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
		const _tchar* ProtoTextureName, _int Count, _float fAccel, _vec3 vRenderPos, _vec3 vScale);

private:
	virtual			void		Free() override;

};

