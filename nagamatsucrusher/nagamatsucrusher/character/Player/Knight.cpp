#include "DxLib.h"
#include "Knight.h"
#include "Pad.h"
#include "EffekseerForDXLib.h"
#include <cmath>

#define PI    3.1415926535897932384626433832795f

namespace 
{
	//�����蔻��̑傫��
	//���a
	constexpr float kCapsuleRadius = 400.0f;


	//�A�j���[�V����	
	constexpr int kWaitAnimIndex = 1;			//�ҋ@��
	constexpr int kRunAnimIndex = 2;			//����
	constexpr int kAttack1AnimIndex = 30;		//1�i�K�ڂ̍U��
	constexpr int kAttack2AnimIndex = 32;		//2�i�K�ڂ̍U��
	constexpr int kAttack3AnimIndex = 33;		//3�i�K�ڂ̍U��
	constexpr int kAttack4AnimIndex = 31;		//4�i�K�ڂ̍U��
	constexpr int kAttack5AnimIndex = 37;		//5�i�K�ڂ̍U��
	constexpr int kAttack6AnimIndex = 36;		//6�i�K�ڂ̍U��

	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeFrame =4.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//�A�j���[�V�����̑��x
	constexpr float kAnimSpeed = 0.75f;

	//�_���[�W��
	constexpr int kDamage = 5;

	//�ړ���
	constexpr float kSpeed = 20.0f;

	//�����Ă���������o���v�Z
	constexpr float kAngle =  180.0f / PI;

	//�v���C���[�̓����蔻��̒���
	constexpr float kAdj = 45.0f;	
	
	//�U���̓����蔻��̒���
	constexpr float kAttackLeftXAdj = 50.0f;
	constexpr float kAttackBehindXAdj = 120.0f;
	constexpr float kAttackBeforeXAdj = 45.0f;
	constexpr float kAttackRightXAdj = 55.0f;

	constexpr float kAttackLeftZAdj = 45.0f;
	constexpr float kAttacBehindZAdj = 45.0f;
	constexpr float kAttackBeforeZAdj = 50.0f;
	constexpr float kAttackRightZAdj = 150.0f;

	//�����蔻��̑傫��
	constexpr float kModelWidth = 80.0f;
	constexpr float kModelHeight = 100.0f;
	constexpr float kModelDepth = 100.0f;

	//�������W
	constexpr float kPosY = 120.0f;
	constexpr float kPosX = -8000.0f;

	//�ő�܂ł����߂�Ƃ���
	constexpr float kMaxZ = 1490.0f;

	constexpr float kMaxX = 12000.0f;

	constexpr float kMinX = -8600.0f;

	//���f���̃T�C�Y�ύX
	constexpr float kExpansion = 100.0f;

	//�d�͂̏����l
	constexpr float kGravity = 0.18f;

	//�W�����v�����Ƃ��ɂ�����d��
	constexpr float kGravityPlus = 0.25f;

	//�m�b�N�o�b�N
	constexpr float kKnockback = 100.0f;


	//�v���C���[�̏ꏊ�ɂ���ăG�l�~�[�𐶐�����
	constexpr float kStage1playerPosX = -4500.0f;
	constexpr float kStage2playerPosX = 265.0f;
	constexpr float kStage3playerPosX = 6175.0f;
	constexpr float kStage4playerPosX = 12000.0f;

	//HP
	constexpr int kHpMax =100;

	//HP1�ɑ΂��Ăǂꂾ���o�[��L�΂���
	constexpr float kDrawSize = 3.5f;

	//���G����
	constexpr int  kInvincibleTime = 180;

	//�񕜗�
	constexpr int kRecovery = 20;
}

Knight::Knight() :m_state(kWait), m_animBlendRate(-1), m_currentAnimNo(-1), m_prevAnimNo(-1), m_count(0)
, m_lightPos(VGet(0.0f, 0.0f, 0.0f)),m_killSE(-1),m_damageSE(-1),m_isRun(false),m_pos(VGet(0.0f,0.0f,0.0f))
, m_move(VGet(0.0f, 0.0f, 0.0f)), m_playerAngle(0), m_movementDirection(VGet(0.0f,0.0f,0.0f)),m_knockBack(0)
,m_bloodHandle(-1)
{
	
}

Knight::~Knight()
{
	MV1DeleteModel(m_handle);

	DeleteSoundMem(m_damageSE);

	DeleteSoundMem(m_killSE);
	
	DeleteEffekseerEffect(m_bloodHandle);
}

void Knight::Init()
{
	//���f���̃��[�h
	m_handle = MV1LoadModel("data/model/player/knight.mv1");

	//���f���̃T�C�Y�ύX
	MV1SetScale(m_handle, VGet(kExpansion, kExpansion, kExpansion));

	//�A�j���[�V�����̏����ݒ�
	m_currentAnimNo = MV1AttachAnim(m_handle, kWaitAnimIndex, -1, true);

	//�����ʒu
	m_pos = VGet(kPosX, kPosY, 0.0f);

	//�N���A�����񐔂�������
	m_stageClear = 0;

	// �Đ����̃G�t�F�N�g�̃n���h��������������B
	m_playingEffectHandle = -1;

	//�v���C���[���G�ɓ����������̃G�t�F�N�g
	m_bloodHandle = LoadEffekseerEffect("data/effect/blood.efkefc", 1.0f);

	//���Ŏa�������̉�
	m_killSE = LoadSoundMem("data/SE/kill.mp3");

	//�G�Ɠ����������̉�
	m_damageSE = LoadSoundMem("data/SE/damage.mp3");

	//�ŏ��͑҂��Ă�����
	m_state = kWait;

	//�ŏ��͑O�������Ă�����
	m_direction = kBefore;

	//HP�̏�����
	m_hp = kHpMax;

	//�m�b�N�o�b�N�̒l
	m_knockBack = 0;

	//���G���Ԃ̏�����
	m_invincibleTime = kInvincibleTime;


	//�t���O�����̏�����
	m_isMove = false;
	m_isAttacking = false;
	m_isAttack = false;
	m_isRun = false;
	m_isHp0 = false;
	m_isHit = false;
}

void Knight::Update(float cameraAngle)
{
	//����֘A
	Operarion(cameraAngle);

	//�A�j���[�V����
	Animation();

	//�X�e�[�W�O�ɏo�Ȃ��悤�ɂ���
	StageProcess();

	//�{�^���J�E���g����莞�Ԃŏ���������
	ButtonCountProcess();

	//Hp��0�ɂȂ������ǂ���
	HpManager();

	//�ǂ����������Ă��邩
	DirectionFacing();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();

	//���f���̈ʒu�X�V
	MV1SetPosition(m_handle, m_pos);

	//���G���Ԋ֌W
	InvincibleTime();

	//���̂̓����蔻��
	m_playerCollision.SetCenter(m_pos.x- kAdj, m_pos.y, m_pos.z- kAdj, kModelWidth, kModelHeight, kModelDepth);
}

void Knight::Draw()
{
	//���f���̕\��
	MV1DrawModel(m_handle);

	//HP�o�[�̕\��
	DrawHPBar();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D();


#ifdef _DEBUG
	DrawFormatString(160, 400, GetColor(255, 255, 255), "player�̍��W(%.2f,%.2f,%.2f)", m_pos.x, m_pos.y, m_pos.z);

	m_playerCollision.Draw(0x000000, true);

	m_attackCollision.Draw(0x000000, true);

	DrawFormatString(0, 100, 0xffffff, "%d", m_isRun);
#endif
}

//�X�e�[�W�O�ɏo�Ȃ��悤�ɂ���
void Knight::StageProcess()
{
	//�X�e�[�W�O�ɂłȂ��悤�ɂ���
	if (m_pos.z > kMaxZ)
	{
		m_pos.z = kMaxZ;
	}
	else if (m_pos.z < -kMaxZ)
	{
		m_pos.z = -kMaxZ;
	}

	if (m_pos.y <= kPosY)
	{
		m_pos.y = kPosY;
	}

	if (m_stageClear == 0)
	{
		if (m_pos.x < kMinX)
		{
			m_pos.x = kMinX;
		}
	}
	if (m_stageClear == 1)
	{
		if (m_pos.x < kStage1playerPosX)
		{
			m_pos.x = kStage1playerPosX;
		}
	}
	if (m_stageClear == 2)
	{
		if (m_pos.x < kStage2playerPosX)
		{
			m_pos.x = kStage2playerPosX;
		}
	}
	if (m_stageClear == 3)
	{
		if (m_pos.x < kStage3playerPosX)
		{
			m_pos.x = kStage3playerPosX;
		}
	}
	else if (m_pos.x > kMaxX)
	{
		m_pos.x = kMaxX;
	}
}

//�ǂ��������Ă���̂�
void Knight::DirectionFacing()
{
	int rotationDegrees = 0.0f;

	rotationDegrees = m_playerAngle * kAngle;

	if (rotationDegrees > -30 && rotationDegrees < 30)
	{
		m_direction = kRight;
	}
	else if (rotationDegrees < 130 && rotationDegrees > 70)
	{
		m_direction = kBehind;
	}
	else if (rotationDegrees < -60 && rotationDegrees > -120)
	{
		m_direction  = kBefore;
	}
	else if (rotationDegrees > 135 || rotationDegrees < -130)
	{
		m_direction = kLeft;
	}
}

//�{�^�����������񐔂���莞�Ԃŏ���������
void Knight::ButtonCountProcess()
{
	//�{�^���̃J�E���g����莞�Ԃŏ���������
	m_count++;
	if (m_count >= 180)
	{
		m_attackCollision.SetCenter(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		m_countXButton = 0;
		m_count = 0;
	}
}

//�{�^���̊��蓖��
void Knight::Operarion(float cameraAngle)
{
	m_move = VGet(0.0f, 0.0f, 0.0f);

	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		m_state = kMove;
		m_move.x += kSpeed;

		m_attackCollision.SetCenter(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}
	if (Pad::IsRelase(PAD_INPUT_RIGHT))
	{
		m_state = kWait;
	}

	//�ړ�
	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		m_state = kMove;
		m_move.x -= kSpeed;

		m_attackCollision.SetCenter(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}
	if (Pad::IsRelase(PAD_INPUT_LEFT))
	{
		m_state = kWait;
	}

	//�ړ�
	if (Pad::IsPress(PAD_INPUT_UP))
	{
		m_state = kMove;
		m_move.z += kSpeed;

		m_attackCollision.SetCenter(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}
	if (Pad::IsRelase(PAD_INPUT_UP))
	{		
		m_state = kWait;
	}

	//�ړ�
	if (Pad::IsPress(PAD_INPUT_DOWN))
	{
		m_state = kMove;
		m_move.z -= kSpeed;

		m_attackCollision.SetCenter(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}
	if (Pad::IsRelase(PAD_INPUT_DOWN))
	{
		m_state = kWait;	
	}
	
	//�R���g���[���[��X�{�^��
	if (Pad::IsTrigger(PAD_INPUT_C))
	{
		m_state = kAttack;
		m_isAttack = true;
		Attack();
		m_countXButton++;
	}
	if (Pad::IsRelase(PAD_INPUT_C))
	{
		m_state = kWait;
	}

	// �ړ��x�N�g�����[���łȂ��ꍇ�A�v���C���[�̌������X�V
	if (m_move.x != 0.0f || m_move.z != 0.0f) 
	{
		m_move = VNorm(m_move);

		// �J�����̉�]����ɕ�����ϊ�
		m_rotMatrix = MGetRotY(cameraAngle);
		m_movementDirection = VTransform(m_move, m_rotMatrix);
		
		//�v���C���[�̈ړ�����
		m_pos = VAdd(m_pos, VScale(m_movementDirection, kSpeed));

		// �v���C���[�̌�����ݒ�
		m_playerAngle = atan2f(-m_movementDirection.x, -m_movementDirection.z);
	}

	//���f���̉�]�X�V
	MV1SetRotationXYZ(m_handle, VGet(0.0f, m_playerAngle, 0.0f));
}

//�A�j���[�V����
void Knight::Animation()
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


	if (m_state == kWait)
	{
		m_isRun = false;
		m_isAttack = false;
		m_isMove = false;
		m_isAttacking = false;

		//�ҋ@���Ȃ�ҋ@���[�V�������s��
		if (isLoop)
		{
			ChangeAnim(kWaitAnimIndex);
		}
	}
	if (m_state == kMove)
	{
		m_isRun = true;
		//�����Ă���Ȃ瑖�郂�[�V�������s��
		if (m_isMove != m_isRun)
		{
			m_isMove = m_isRun;
			if (m_isMove)
			{
				ChangeAnim(kRunAnimIndex);
			}
		}
	}
	if (m_state == kAttack)
	{
		if (m_isAttacking != m_isAttack)
		{
			m_isAttacking = m_isAttack;
			if (m_isAttacking && m_countXButton == 1)
			{
				ChangeAnim(kAttack1AnimIndex);
			}
			if (m_isAttacking && m_countXButton == 2)
			{
				ChangeAnim(kAttack2AnimIndex);
			}
			if (m_isAttacking && m_countXButton == 3)
			{
				ChangeAnim(kAttack3AnimIndex);
			}
			if (m_isAttacking && m_countXButton == 4)
			{
				ChangeAnim(kAttack4AnimIndex);
			}
			if (m_isAttacking && m_countXButton == 5)
			{
				ChangeAnim(kAttack5AnimIndex);
			}
			if (m_isAttacking && m_countXButton == 6)
			{
				ChangeAnim(kAttack6AnimIndex);
				m_countXButton = 0;
			}
		}
	}
}

bool Knight::UpdateAnim(int attachNo)
{
	//�A�j���[�V�������ݒ肳��Ă��Ȃ��̂ŏI��
	if (attachNo == -1) return false;

	//�A�j���[�V������i�s������
	float now = MV1GetAttachAnimTime(m_handle, attachNo);

	//�A�j���[�V�����i�߂�
	now += kAnimSpeed;

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

void Knight::ChangeAnim(int animIndex)
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

//�U��
void Knight::Attack()
{
	PlaySoundMem(m_killSE, DX_PLAYTYPE_BACK, true);

	if (m_direction == kBefore)
	{
		m_attackCollision.SetCenter(m_pos.x + kAttackBeforeXAdj, m_pos.y, m_pos.z - kAttackBeforeZAdj, kModelWidth, kModelHeight, kModelDepth);
	}
	if (m_direction == kBehind)
	{
		m_attackCollision.SetCenter(m_pos.x - kAttackBehindXAdj, m_pos.y, m_pos.z - kAttacBehindZAdj, kModelWidth, kModelHeight, kModelDepth);
	}
	if (m_direction == kRight)
	{
		m_attackCollision.SetCenter(m_pos.x - kAttackRightXAdj, m_pos.y, m_pos.z - kAttackRightZAdj, kModelWidth, kModelHeight, kModelDepth);
	}
	if (m_direction == kLeft)
	{
		m_attackCollision.SetCenter(m_pos.x - kAttackLeftXAdj, m_pos.y, m_pos.z + kAttackLeftZAdj, kModelWidth, kModelHeight, kModelDepth);
	}

}

//HP�o�[�̕`��
void Knight::DrawHPBar()
{
	// HP �̒l�𕶎���ƃo�[�ŕ\��
	DrawFormatString(50, 50, GetColor(255, 255, 255), "%4d", m_hp);
	// HP �̒l���̑傫�������l�p�Ɏ��܂�悤�ɒl��傫�����܂�
	DrawFillBox(200, 50, 200 + m_hp	 * kDrawSize, 66, GetColor(77, 181, 106));
	DrawLineBox(200, 50, 450 + kHpMax, 66, GetColor(77, 181, 0));
}

//���G����
void Knight::InvincibleTime()
{
	if(m_isHit)
	{
		m_invincibleTime--;
	}


	if (m_invincibleTime <= 0)
	{
		m_isHit =false;
		m_invincibleTime = kInvincibleTime;
	}
}

void Knight::LeaningBack()
{
	if (m_direction == kRight)
	{
		m_pos.z += m_knockBack;
	}
	if (m_direction == kBehind)
	{
		m_pos.x += m_knockBack;
	}
	if (m_direction == kBefore)
	{
		m_pos.x -= m_knockBack;
	}
	if (m_direction == kLeft)
	{
		m_pos.z -= m_knockBack;
	}

}

//�G�Ɠ����������̏���
void Knight::HitBody()
{	
	if (!m_isHit)
	{
		m_isHit = true;
		
		m_hp = m_hp-kDamage;

		PlaySoundMem(m_damageSE, DX_PLAYTYPE_BACK);

		m_knockBack++;

		// �G�t�F�N�g���Đ�����B
		m_playingEffectHandle = PlayEffekseer3DEffect(m_bloodHandle);

		
		// �Đ����̃G�t�F�N�g���ړ�����B
		SetPosPlayingEffekseer3DEffect(m_playingEffectHandle, m_pos.x, m_pos.y + 100, m_pos.z);

		if (m_knockBack <= kKnockback)
		{
			m_knockBack = 0;
		}

	}
}

//HP��0�ɂȂ������̏���
void Knight::HpManager()
{
	if (m_hp <= 0)
	{
		m_isHp0 -= true;
	}
}

//�N���A������X�e�[�W�O�ɏo��ʒu���A��
void Knight::ConditionCleared(bool clear)
{
	if (clear)
	{
		m_stageClear++;

		m_hp += kRecovery;

		if (m_hp >= kHpMax)
		{
			m_hp = kHpMax;
		}
	}
	else
	{
		if (m_stageClear == 0)
		{
			if (m_pos.x > kStage1playerPosX)
			{
				m_pos.x = kStage1playerPosX;
			}
		}
		if (m_stageClear == 1)
		{
			if (m_pos.x > kStage2playerPosX)
			{
				m_pos.x = kStage2playerPosX;
			}
		}
		if (m_stageClear == 2)
		{
			if (m_pos.x > kStage3playerPosX)
			{
				m_pos.x = kStage3playerPosX;
			}
		}
		if (m_stageClear == 3)
		{
			if (m_pos.x > kStage4playerPosX)
			{
				m_pos.x = kStage4playerPosX;
			}
		}
	}
}

