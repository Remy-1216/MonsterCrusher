#pragma once
#include "Rect.h"
class Knight
{
public:
	Knight();
	virtual ~Knight();

	void Init();
	void Update(VECTOR cameraPos);
	void Draw();

	//�G�Ɠ����������̏���
	void HitBody();

	//Hp���ɂȂ����畉��
	void HpManager();

	//�������N���A���Ȃ��Ɛ�ɐi�߂Ȃ�
	void ConditionCleared(bool clear);
	
	//Hp��0�ɂȂ������ǂ�����^����
	bool GetIsHp0() const noexcept {return m_isHp0;}

	//�v���C���[�̍��W�ʒu��^����
	VECTOR GetPlayerPos() const noexcept { return m_pos; }

	//�v���C���[�̓����蔻���^����
	Rect GetPlayerCollision() const noexcept { return m_playerCollision; }

	//�U���̓����蔻���^����
	Rect GetAttackCollision() const noexcept { return m_attackCollision; }

private:

	//����
	void Operarion(VECTOR cameraPos);
	//�W�����v���̏���
	void JumpProcess();

	//�X�e�[�W�O�ɏo�Ȃ��悤�ɂ��鏈��
	void StageProcess();

	//�{�^�����������Ƃ��̃J�E���g����莞�ԉ߂���Ə��������鏈��
	void ButtonCountProcess();

	//�A�j���[�V�����֘A
	void  Animation();

	//�A�j���[�V�����̐i�s
	//���[�v�������ǂ�����Ԃ�
	bool UpdateAnim(int attachNo);

	//�A�j���[�V�����̕ύX
	void ChangeAnim(int animIndex);

	//�ʏ�U��
	void Attack();

	//HP�o�[�\��
	void DrawHPBar();

	//���@�U��
	//void AttackMagic();

private:

	//�v���C���[�̏��
	enum  state
	{
		kWait,			//�ҋ@��
		kMove,			//�����Ă���
		kAttack,		//�U����
		kAerialSlash,	//�󒆎a�蒆
		kAttackMagic	//���@�U����
	};

	//�v���C���[���ǂ���������Ă��邩
	enum direction
	{
		kRight,
		kLeft,
		kUp,
		kDown
	};

private:

	//HP
	int m_hp;

	//���f���̃n���h��
	int m_handle;

	//���Ő؂�������SE
	int m_killSE;

	//�_���[�W�𕉂�������SE
	int m_damageSE;

	//���
	int m_state;

	//�����Ă������
	int m_direction;

	//A�{�^�������񉟂�����
	int m_countAButton;

	//X�{�^�������񉟂�����
	int m_countXButton;

	//�X�e�[�W�N���A
	int m_stageClear;

	//�d��
	float m_gravity;

	//�{�^���̃��Z�b�g
	int m_count;


	//�A�j���[�V�������
	int m_currentAnimNo;	//���݂̃A�j���[�V����
	int m_prevAnimNo;		//�ύX�O�̃A�j���[�V����
	float m_animBlendRate;	//�A�j���[�V������������

	//�����Ă��邩��ێ�����
	bool m_isMove;

	//�U�����Ă��邩��ێ�����
	bool m_isAttacking;

	//�����Ă��邩�ǂ���
	bool m_isRun;

	//�U�����Ă��邩�ǂ���
	bool m_isAttack;

	//�W�����v���Ă��邩�ǂ���
	bool m_isJump;

	//Hp��0�ɂȂ������ǂ���
	bool m_isHp0;

	//���W
	VECTOR m_pos;

	//�J�����̈ʒu
	VECTOR m_cameraPos;

	//���C�g�̈ʒu
	VECTOR m_lightPos;

	//�����蔻��
	//�v���C���[�̓����蔻��
	Rect m_playerCollision;

	//�U���̓����蔻��
	Rect m_attackCollision;
};

