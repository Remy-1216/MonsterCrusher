#pragma once
#include "EnemyBase.h"
class Boss :public EnemyBase
{
public:
	Boss(int handle,VECTOR playerPos);
	virtual ~Boss();

	void Update(Knight* knight, VECTOR playerPos)override;

	//�v���C���[�̈ʒu�ɂ���Đ�������ʒu��ς���
	void SetPosX(VECTOR playerPos)override;

private:
	//�U��
	void Attack(Knight* knight);

	//�ړ��֘A
	void Move(Knight* knight);

	//�A�j���[�V�����֘A
	void  Animation();

	//���[�v�������ǂ�����Ԃ�
	bool UpdateAnim(int attachNo);

	//�A�j���[�V�����̕ύX
	void ChangeAnim(int animIndex);

private:
	//HP
	int m_hp;

	//�C�e�̃��f��
	int m_cannonballHandle;

	//�A�j���[�V�������
	int m_currentAnimNo;	//���݂̃A�j���[�V����
	int m_prevAnimNo;		//�ύX�O�̃A�j���[�V����
	float m_animBlendRate;	//�A�j���[�V������������

	//�����_���Ȉʒu
	float m_randomPosX;

	//�C�e�̒��e�ʒu
	VECTOR m_impactPos;
};

