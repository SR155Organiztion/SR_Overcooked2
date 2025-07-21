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

	void						Set_Star(_uint _iID);
	void						Set_Angle(const _float _fAngle);
	void						Set_Scale(const _vec3 _vScale);
	void						Set_StageName(string _s);
	void						Set_StageNum(_int _iStageNum) {

		m_iStageNum = _iStageNum;
	}

	_int						Get_StageNum() {
		return m_iStageNum;
	}

	_vec3 Get_Pos();
private:
	HRESULT		Add_Component();
	HRESULT		Set_Metarial();
	

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	//��� �ؽ���
	//�ؽ��� 0~3�� ���� ����, 4�� ���� �÷��̸� ���� ������ �ǹ�.
	_uint					m_iStarNum;
	//�������� �̸� ����
	string					m_szStage;
	_int					m_iStageNum;

public:
	static CFlag* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

