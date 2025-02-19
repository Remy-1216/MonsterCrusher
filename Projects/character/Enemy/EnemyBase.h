#pragma once
#include "DxLib.h"
#include "Rect.h"
class Knight;
class EnemyBase
{
public:
	EnemyBase(int ModelHandle);
	virtual ~EnemyBase();

	void Init();
	
	virtual void Update(Knight* knight, VECTOR playerPos) abstract;
	
	void Draw();	//���[�v�������ǂ�����Ԃ�

	//�v���C���[�̈ʒu�ɂ���Đ�������ʒu��ς���
	virtual void SetPosX(VECTOR playerPos)abstract;

	//�U�����󂯂������ɂ���ăm�b�N�o�b�N����
	void Knockback(int direction);

	void End();

	//���W�̐ݒ�
	void SetPos(VECTOR mpos) { m_pos = mpos; }
protected:

	//�G�l�~�[�̏��
	enum  state
	{
		kMove,			//�����Ă���
		kAttack,		//�U����
		kDamage,		//�_���[�W�󂯂Ă���
		kDeath			//����
	};

	//�G�l�~�[���ǂ���������Ă��邩
	enum direction
	{
		kRight,					//�E
		kLeft,					//��
		kBefore,				//�O
		kBehind,				//���

	};
protected:

	//���
	int m_state;

	int	m_animFrame;

	//HP
	int m_hp;

	//�U����
	int m_attackPower;

	//���f���̃n���h��
	int m_handle;

	//�U�����󂯂�����SE
	int m_damageSE;

	//�o���G�t�F�N�g�̃n���h��
	int m_bloodHandle;

	//�G�t�F�N�g�̍Đ�����
	int m_timer;

	//�Đ����̃G�t�F�N�g�n���h��
	int m_playingEffectHandle;

	//�����Ă������
	int m_direction;

	//�v���C���[�̌����Ă������
	int m_playerDirection;

	//�m�b�N�o�b�N
	int m_knockback;

	//���G����
	int m_invincibleTime;

	//�G�l�~�[�̌����Ă������
	float m_enemyAngle;

	//�U���ɓ����������ǂ���
	bool m_isHitAttack;

	//�v���C���[�ɓ����������ǂ���
	bool m_isHitPlayer;

	//���W
	VECTOR	 m_pos;

	//�����蔻��
	Rect m_enemyCollision;

	//�v���C���[�̓����蔻��
	Rect m_playerAttackCollision;

};

