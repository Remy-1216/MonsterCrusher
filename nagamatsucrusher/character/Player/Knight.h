#pragma once
#include "Rect.h"
class Knight
{
public:
	Knight();
	virtual ~Knight();

	void Init();
	void Update(float cameraAngle);
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
	void Operarion(float cameraAngle);

	//�X�e�[�W�O�ɏo�Ȃ��悤�ɂ��鏈��
	void StageProcess();

	//�ǂ��������Ă���̂�
	void DirectionFacing();

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

	//���G
	void InvincibleTime();

	//�̂�����
	void LeaningBack();

private:

	//�v���C���[�̏��
	enum  state
	{
		kWait,			//�ҋ@��
		kMove,			//�����Ă���
		kAttack,		//�U����
	};

	//�v���C���[���ǂ���������Ă��邩
	enum direction
	{
		kRight,					//�E
		kLeft,					//��
		kBefore,				//�O
		kBehind,				//���
		kDiagonallyRightFront,	//�E�΂ߑO
		kDiagonallyLeftFront,	//���΂ߑO
		kRightDiagonalBack,		//�E�΂ߌ��
		kLeftDiagonalBack		//���΂ߌ��

	};

private:

	//HP
	int m_hp;

	//���f���̃n���h��
	int m_handle;

	//�o���G�t�F�N�g�̃n���h��
	int m_bloodHandle;

	//���Ő؂�������SE
	int m_killSE;

	//�_���[�W�𕉂�������SE
	int m_damageSE;

	//�Đ����̃G�t�F�N�g�n���h��
	int m_playingEffectHandle;

	//�G�t�F�N�g�̍Đ�����
	int m_timer;

	//���݂̏��
	int m_state;

	//�m�b�N�o�b�N
	int m_leaningBack;

	//�����Ă������
	int m_direction;

	//X�{�^�������񉟂�����
	int m_countXButton;

	//�X�e�[�W�N���A
	int m_stageClear;

	//�{�^���̃��Z�b�g
	int m_count;

	//���G����
	int m_invincibleTime;

	//�d��
	float m_gravity;

	//�A�j���[�V�������
	int m_currentAnimNo;	//���݂̃A�j���[�V����
	int m_prevAnimNo;		//�ύX�O�̃A�j���[�V����
	float m_animBlendRate;	//�A�j���[�V������������

	//�v���C���[�̌����Ă������
	float m_playerAngle;

	//�����Ă��邩�ǂ���
	bool m_isRun;

	//�����Ă��邩��ێ�����
	bool m_isMove;

	//�U�����Ă��邩�ǂ���
	bool m_isAttack;
	
	//�U�����Ă��邩��ێ�����
	bool m_isAttacking;

	//�G�Ƃ����������ǂ���
	bool m_isHit;

	//Hp��0�ɂȂ������ǂ���
	bool m_isHp0;

	//���W
	VECTOR m_pos;

	//�ړ���
	VECTOR m_move;

	//�J�����̉�]������ɕϊ������l��ۑ�����
	VECTOR m_movementDirection;

	//�J�����̑O���x�N�g��
	VECTOR m_cameraForward;

	//�J�����̉E�����x�N�g��
	VECTOR m_cameraRight;

	//���C�g�̈ʒu
	VECTOR m_lightPos;

	//�J�����̃}�g���b�N�X
	MATRIX m_rotMatrix;

	// ���f���̉�]�s����쐬
	MATRIX m_modelRotMatrix;

	//�����蔻��
	//�v���C���[�̓����蔻��
	Rect m_playerCollision;

	//�U���̓����蔻��
	Rect m_attackCollision;
};

