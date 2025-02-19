#include "Slime.h"
#include "Knight.h"
#include <math.h>
#include "EffekseerForDXLib.h"

#define PI    3.1415926535897932384626433832795f

namespace
{
	//�G�̑���
	constexpr float kSpeed = 2.0f;

	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeFrame = 4.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//�G�̍ő�HP
	constexpr int kMaxHp = 50;

	//�_���[�W��
	constexpr int kDamageNum = 10;

	//�A�j���[�V����
	constexpr int kRunAnimIndex = 6;

	//�����ʒu
	constexpr int kAppearanceX = 750;

	//�������W
	constexpr float kPosY = 120.0f;

	//�����蔻��̒���
	constexpr float kAdj = 45.0f;

	//�����蔻��̑傫��
	constexpr float kModelWidth = 80.0f;
	constexpr float kModelHeight = 100.0f;
	constexpr float kModelDepth = 100.0f;

	//�v���C���[�̈ʒu
	constexpr float kPlayerPos1X = -4500.0f;
	
	constexpr float kPlayerPos2X = 265.0f;
	
	constexpr float kPlayerPos3X = 6175.0f;

	constexpr float kPlayerPos4X = 12000.0f;

	//�m�b�N�o�b�N
	constexpr float kKnockback = 50.0f;

	constexpr float kMaxKnockback = 250.0f;

	//���G����
	constexpr int  kInvincibleTime = 20;

	//�����Ă���������o���v�Z
	constexpr float kAngle = 180.0f / PI;

	//�����Ă������
	constexpr int kRotationStandard1 = 30;

	constexpr int kRotationStandard2 = 120;
}

Slime::Slime(int m_handle,VECTOR playerPos) :EnemyBase(m_handle), m_animBlendRate(-1), m_currentAnimNo(-1), m_prevAnimNo(-1)
{
	//�A�j���[�V�����̏����ݒ�
	m_currentAnimNo = MV1AttachAnim(m_handle, kRunAnimIndex, -1, true);

	//HP�̏�����
	m_hp = kMaxHp;

	//���G���Ԃ̐ݒ�
	m_invincibleTime = kInvincibleTime;

	//������
	m_isHitAttack = false;

	m_knockback = 0;

	//���W�̐ݒ�
	SetPosX(playerPos);
}

Slime::~Slime()
{
}

void Slime::Update(Knight* knight, VECTOR playerPos)
{
	if (!m_isHitAttack)
	{
		//�A�j���[�V����
		Animation();

		//�v���C���[�ɋ߂Â�
		ComingPlayer(knight);
	}

	//�����Ă������
	DirectionFacing();

	//���G����
	InvincibleTime();

	m_enemyCollision.SetCenter(m_pos.x- kAdj, m_pos.y, m_pos.z - kAdj, kModelWidth, kModelHeight,kModelDepth);
}

void Slime::SetPosX(VECTOR playerPos)
{
	if (playerPos.x < kPlayerPos1X)
	{
		m_randomPosX = GetRand(kAppearanceX);
		m_randomPosX = m_randomPosX + playerPos.x;
		m_pos = VGet(m_randomPosX, kPosY, 0.0f);
	}
	else if (playerPos.x < kPlayerPos2X)
	{
		m_randomPosX = GetRand(kAppearanceX);
		m_randomPosX = m_randomPosX + playerPos.x;
		m_pos = VGet(m_randomPosX, kPosY, 0.0f);
	}
	else if (playerPos.x < kPlayerPos3X)
	{
		m_randomPosX = GetRand(kAppearanceX);
		m_randomPosX = m_randomPosX + playerPos.x;
		m_pos = VGet(m_randomPosX, kPosY, 0.0f);
	}
	else if (playerPos.x < kPlayerPos4X)
	{
		m_randomPosX = GetRand(kAppearanceX);
		m_randomPosX = m_randomPosX + playerPos.x;
		m_pos = VGet(m_randomPosX, kPosY, 0.0f);
	}
}

void Slime::ComingPlayer(Knight* knight)
{
	VECTOR playerPos = knight->GetPlayerPos();
	//�G�̏����ʒu����^�[�Q�b�g�ʒu�Ɍ������x�N�g���𐶐�����
	//�n�_����I�_�Ɍ������x�N�g�������߂�ꍇ�A�I�_�̍��W-�n�_�̍��W�ŋ��߂�
	VECTOR toTarget = VSub(playerPos, m_pos);

	//�x�N�g���̒�����kSpeed�ɂ��Ă��
	//�x�N�g���̐��K���@�������P�ɂ���
	toTarget = VNorm(toTarget);
	//kSpeed�ł�����
	m_distance.x = toTarget.x * kSpeed;
	m_distance.y = toTarget.y;
	m_distance.z = toTarget.z * kSpeed;

	m_pos = VAdd(m_pos, m_distance);


	// �R�c���f���Q����R�c���f���P�Ɍ������x�N�g�����Z�o
	VECTOR SubVector = VSub(playerPos, m_pos);

	// atan2 ���g�p���Ċp�x���擾
	m_enemyAngle = atan2(SubVector.x, SubVector.z);

	//�v���C���[�̕���������
	MV1SetRotationXYZ(m_handle, VGet(0.0f, m_enemyAngle + DX_PI_F, 0.0f));

	//�|�W�V�����̐ݒ�
	MV1SetPosition(m_handle, m_pos);
}

void Slime::InvincibleTime()
{
	if (m_isHitAttack)
	{
		m_invincibleTime--;
		Knockback(m_playerDirection);
	}

	if (m_invincibleTime <= 0)
	{
		m_invincibleTime = kInvincibleTime;
		m_isHitAttack = false;
	}
}

void Slime::DirectionFacing()
{
	int rotationDegrees = 0.0f;

	rotationDegrees = m_enemyAngle * kAngle;

	if (rotationDegrees > -kRotationStandard1 && rotationDegrees < kRotationStandard1)
	{
		m_direction = kRight;
	}
	else if (rotationDegrees < -kRotationStandard1 && rotationDegrees > -kRotationStandard2)
	{
		m_direction = kBefore;
	}
	else if (rotationDegrees > kRotationStandard1 && rotationDegrees < kRotationStandard2)
	{
		m_direction = kBehind;
	}

	else if (rotationDegrees > kRotationStandard2 || rotationDegrees < -kRotationStandard1)
	{
		m_direction = kLeft;
	}
}

void Slime::Animation()
{
	if (m_prevAnimNo != -1)
	{
		//test 8�t���[���Ő؂�ւ�
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= 1.0f) m_animBlendRate = 1.0f;
		//�ύX��̃A�j���[�V����������ݒ肷��
		MV1SetAttachAnimBlendRate(m_handle, m_prevAnimNo, 1.0f - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_handle, m_currentAnimNo, m_animBlendRate);
	}
	bool isLoop = UpdateAnim(m_currentAnimNo);
	UpdateAnim(m_prevAnimNo);

	if (m_state == kMove)
	{
		if (isLoop)
		{
			ChangeAnim(kRunAnimIndex);
		}
	}
}
bool Slime::UpdateAnim(int attachNo)
{
	//�A�j���[�V�������ݒ肳��Ă��Ȃ��̂ŏI��
	if (attachNo == -1) return false;

	//�A�j���[�V������i�s������
	float now = MV1GetAttachAnimTime(m_handle, attachNo);

	//�A�j���[�V�����i�߂�
	now += 0.5f;

	//���ݍĐ����̃A�j���[�V�����̑��J�E���g���擾����
	float total = MV1GetAttachAnimTotalTime(m_handle, attachNo);
	bool isLoop = false;

	while (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	//�i�߂����Ԃɐݒ�
	MV1SetAttachAnimTime(m_handle, attachNo, now);
	return isLoop;
}

void Slime::ChangeAnim(int animIndex)
{
	//����ɌÂ��A�j���[�V�������A�^�b�`����Ă���ꍇ�͂��̎��_�ō폜���Ă���
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_handle, m_prevAnimNo);
	}

	//���ݍĐ����̑ҋ@�A�j���[�V�����͕ύX�O�̃A�j���[�V����������
	m_prevAnimNo = m_currentAnimNo;

	//�ύX��̃A�j���[�V�����Ƃ��čU���A�j���[�V���������߂Đݒ肷��
	m_currentAnimNo = MV1AttachAnim(m_handle, animIndex, -1, false);

	//�؂�ւ��̏u�Ԃ͕ύX�O�̃A�j���[�V�������Đ�������Ԃɂ���
	m_animBlendRate = 0.0f;

	//�ύX�O�̃A�j���[�V����100%
	MV1SetAttachAnimBlendRate(m_handle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//�ύX��̃A�j���[�V����0%
	MV1SetAttachAnimBlendRate(m_handle, m_currentAnimNo, m_animBlendRate);
}

void Slime::HitAttack(Knight* knight)
{
	m_playerAttackCollision = knight->GetAttackCollision();
	m_playerDirection = knight->GetDirection();
	if (m_enemyCollision.IsCollsion(m_playerAttackCollision) && !m_isHitAttack)
	{

		PlaySoundMem(m_damageSE, DX_PLAYTYPE_BACK);

		m_hp -= kDamageNum;

		


		m_isHitAttack = true;
	}
	else
	{
		m_state = kMove;
	}
}

void Slime::HitPlayer(Knight* knight, Rect playerCollision)
{
	if (m_enemyCollision.IsCollsion(playerCollision))
	{
		knight->HitBody(m_direction);
	}
}
