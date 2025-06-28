#pragma once
#include "CUi.h"
class CUi_Gauge :  public CUi
{
private:
    float m_percent;
    bool m_bActivate; //ȯ�� OBJ�� ��ȣ�ۿ��� ���� ����
	//(ResourceMgr*)m_pResourceSize = 100; ��ü ���ҽ��� size�� ��´�.
	//(ResourceMgr*)m_pResource = 0; ���ݱ��� ���� ���� ���� ��´�.
	LPDIRECT3DTEXTURE9 m_pTexBar; //������ ��ü ���̸�ŭ�� ��
	LPDIRECT3DTEXTURE9 m_pTexGuage; //�������� ǥ��

	float barWidth; //��ü ���� ��
	float barHeight; //��ü ���� ����
	D3DXVECTOR3 pos;  //�� ��ġ

protected:
	CUi_Gauge(LPDIRECT3DDEVICE9 pZGraphicDev);
	CUi_Gauge(const CGameObject& rhs);
	~CUi_Gauge();

public:
	void SetPercent(float p) { m_percent = p; }


};

