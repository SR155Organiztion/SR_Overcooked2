/**
* @mainpage CState ����
* @section intro �Ұ�
* - State�� ���� �ľ�
* @section ����
* - Object�� UpdateŸ�ֿ̹� Update_State�� TestForExit�� ��ȸ
* - TestForExit�� Ż�������� �˻��ϴ� �Լ��� �� �����Ӹ��� ȣ����. �� �� Ż�� ���� ���� ��, Change_State ȣ���� ���⼭ �ϸ� ��
* - Change_State ��, ��ȯ�ϴ� State�� Enter_State ȣ��� ������
* @section ���ǻ���
* - State ���� ��, �� Ŭ���� �ȿ� ���� State�� �����ϴ� ���� ��õ
* -# Idle, Move, Attack �� State�� �������� �ؼ� ���� ���� ����� ������ �ʴ°� ����
* - State�� �ʿ��� �Լ� �� ��� ������ ������ �ִ� ������ ������
* - �� ������ �а� �����ϱ� ����� �� ����
* -# State ���� - CPlayerState.h �� cpp
* -# FSMComponent ���� - CRealPlayer.cpp
*/


#pragma once
#include "CGameObject.h"

class CState
{
public:
	virtual	void		Enter_State(CGameObject* Owner) = 0;
	virtual	void		Update_State(CGameObject* Owner, const _float& fTimeDelta) = 0;
	virtual	void		TestForExit_State(CGameObject* Owner) = 0;
};


