#pragma once
#include "enemybase.h"
#include "Rect.h"
class Slime :public EnemyBase
{
public:
	Slime(int m_handle, VECTOR playerPos);
	virtual ~Slime();

	void Update(Knight* knigh, VECTOR playerPos)override;

	//�v���C���[�̈ʒu�ɂ���Đ�������ʒu��ς���
	void SetPosX(VECTOR playerPos)override;

	//�U���������������̏���
	void HitAttack(Rect playerAttack);

	//�v���C���[�ɂ�����
	void HitPlayer(Knight*knight, Rect playerCollision);

	//�X���C���̓����蔻���^����
	Rect GetSlimeCollision() const noexcept { return m_enemyCollision; }

	//�X���C���̗̑͂�^����
	int GetSlimeHp() const noexcept { return m_hp; }

	//�����������ǂ�����^����
	bool GetHitPlayer() const noexcept { return m_isHitPlayer; }

private:
	//�v���C���[�ɏ����Â߂Â�
	void ComingPlayer(Knight* knight);

	//���G
	void InvincibleTime();

	//�A�j���[�V�����֘A
	void  Animation();

	//���[�v�������ǂ�����Ԃ�
	bool UpdateAnim(int attachNo);

	//�A�j���[�V�����̕ύX
	void ChangeAnim(int animIndex);

	

	
private:
	//�A�j���[�V�������
	int m_currentAnimNo;	//���݂̃A�j���[�V����
	int m_prevAnimNo;		//�ύX�O�̃A�j���[�V����
	float m_animBlendRate;	//�A�j���[�V������������

	//�����_���Ȉʒu
	float m_randomPosX;

	//�i�ދ���
	VECTOR m_distance;
};

