#include "DxLib.h"
#include "Knight.h"
#include "Pad.h"
#include <cmath>

namespace 
{
	//�A�j���[�V����	
	constexpr int kWaitAnimIndex = 1;			//�ҋ@��
	constexpr int kRunAnimIndex = 2;			//����
	constexpr int kAttack1AnimIndex = 30;		//1�i�K�ڂ̍U��
	constexpr int kAttack2AnimIndex = 32;		//2�i�K�ڂ̍U��
	constexpr int kAttack3AnimIndex = 33;		//3�i�K�ڂ̍U��
	constexpr int kAttack4AnimIndex = 31;		//4�i�K�ڂ̍U��
	constexpr int kAttack5AnimIndex = 37;		//5�i�K�ڂ̍U��
	constexpr int kAttack6AnimIndex = 36;		//6�i�K�ڂ̍U��
	constexpr int kAerialSlashAnimIndex = 39;	//�󒆎a��
	constexpr int kAttackMagicAnimIndex = 61;	//���@�U��

	//�W�����v��
	constexpr float kJumpPower = 10.0f;

	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeFrame =4.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//�ړ���
	constexpr float kSpped = 15.0f;

	//��]��
	constexpr float kRotateRight = 270.0f * DX_PI_F / 180.0f;
	constexpr float kRotateLeft = 90.0f * DX_PI_F / 180.0f;
	constexpr float kRotateUp = 180.0f * DX_PI_F / 180.0f;
	constexpr float kRotateDown = 0.0f;

	//�v���C���[�̓����蔻��̒���
	constexpr float kAdj = 45.0f;	
	
	//�U���̓����蔻��̒���
	constexpr float kAttackLeftXAdj = 100.0f;
	constexpr float kAttackRightXAdj = 150.0f;
	constexpr float kAttackUpXAdj = 45.0f;
	constexpr float kAttackDownXAdj = 45.0f;

	constexpr float kAttackLeftZAdj = 45.0f;
	constexpr float kAttackRightZAdj = 45.0f;
	constexpr float kAttackUpZAdj = 50.0f;
	constexpr float kAttackDownZAdj = 150.0f;

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
	constexpr float kKnockback = 250.0f;


	//�v���C���[�̏ꏊ�ɂ���ăG�l�~�[�𐶐�����
	constexpr float kStage1playerPosX = -4500.0f;

	constexpr float kStage2playerPosX = 265.0f;

	constexpr float kStage3playerPosX = 6175.0f;

	constexpr float kStage4playerPosX = 12000.0f;

	//HP
	constexpr int kHpMax = 100;

	//HP1�ɑ΂��Ăǂꂾ���o�[��L�΂���
	constexpr float kDrawSize = 3.5f;
}

Knight::Knight() :m_state(kWait), m_animBlendRate(-1), m_currentAnimNo(-1), m_prevAnimNo(-1), m_count(0)
, m_lightPos(VGet(0.0f, 0.0f, 0.0f)),m_killSE(-1),m_damageSE(-1),m_isRun(false),m_pos(VGet(0.0f,0.0f,0.0f))
{
	m_handle = MV1LoadModel("data/model/player/knight.mv1");
}

Knight::~Knight()
{
	MV1DeleteModel(m_handle);

	DeleteSoundMem(m_damageSE);

	DeleteSoundMem(m_killSE);
}

void Knight::Init()
{
	MV1SetScale(m_handle, VGet(kExpansion, kExpansion, kExpansion));

	//�A�j���[�V�����̏����ݒ�
	m_currentAnimNo = MV1AttachAnim(m_handle, kWaitAnimIndex, -1, true);

	m_pos = VGet(kPosX, kPosY, 0.0f);

	m_stageClear = 0;

	m_killSE = LoadSoundMem("data/SE/kill.mp3");

	m_damageSE = LoadSoundMem("data/SE/damage.mp3");

	m_state = kWait;

	m_direction = kLeft;

	m_hp = kHpMax;

	m_isMove = false;
	m_isAttacking = false;
	m_isAttack = false;
	m_isRun = false;
	m_isHp0 = false;
}

void Knight::Update(VECTOR cameraPos)
{
	//����֘A
	Operarion(cameraPos);

	//�A�j���[�V�����֘A
	Animation();

	//�W�����v������̏���
	JumpProcess();

	//�X�e�[�W�O�ɏo�Ȃ��悤�ɂ���
	StageProcess();

	//�{�^���J�E���g����莞�Ԃŏ���������
	ButtonCountProcess();

	//Hp��0�ɂȂ������ǂ���
	HpManager();

	//���f���̈ʒu�X�V
	MV1SetPosition(m_handle, m_pos);


	m_playerCollision.SetCenter(m_pos.x- kAdj, m_pos.y, m_pos.z- kAdj, kModelWidth, kModelHeight, kModelDepth);
}

void Knight::Draw()
{
	MV1DrawModel(m_handle);

	DrawHPBar();

#ifdef _DEBUG
	DrawFormatString(160, 400, GetColor(255, 255, 255), "player�̍��W(%.2f,%.2f,%.2f)", m_pos.x, m_pos.y, m_pos.z);

	m_playerCollision.Draw(0x000000, true);

	m_attackCollision.Draw(0x000000, true);
#endif
}

//�W�����v�������̏���
void Knight::JumpProcess()
{
	//�W�����v������̏���
	if (m_pos.y > kPosY)
	{
		m_pos = VSub(m_pos, VGet(0, m_gravity, 0));
		m_gravity += kGravityPlus;
	}
	else
	{
		m_gravity = kGravity;
		m_state = kWait;
	}
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

//�{�^�����������񐔂���莞�Ԃŏ���������
void Knight::ButtonCountProcess()
{
	//�{�^���̃J�E���g����莞�Ԃŏ���������
	m_count++;

	if (m_count >= 60)
	{
		m_countAButton = 0;

		
	}
	if (m_count >= 180)
	{
		m_attackCollision.SetCenter(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		m_countXButton = 0;
		m_count = 0;
	}
}

//�{�^���̊��蓖��
void Knight::Operarion(VECTOR cameraPos)
{
	//m_cameraPos.x = cameraPos.x -m_pos.x;

	//m_cameraPos.y = cameraPos.y -m_pos.y;

	//m_cameraPos.z = cameraPos.z -m_pos.z;

	//m_cameraPos.x = m_cameraPos.x * m_cameraPos.x;
	//
	//m_cameraPos.y = m_cameraPos.y * m_cameraPos.y;
	//
	//m_cameraPos.z = m_cameraPos.z * m_cameraPos.z;

	//m_cameraPos = VSub(m_pos, m_cameraPos);

	//m_cameraPos = VNorm(m_cameraPos);

	//m_cameraPos = VScale(m_cameraPos, 180);


	//�ړ�
	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		m_state = kMove;
		m_direction = kDown;
		m_pos = VAdd(m_pos, VGet(0.0f,
			0.0f, 
			-kSpped));

		MV1SetRotationXYZ(m_handle, VGet(0, kRotateDown, 0));

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
		m_direction = kUp;
		m_pos = VAdd(m_pos, VGet(0.0f,
			0.0f,
			kSpped));

		MV1SetRotationXYZ(m_handle, VGet(0,kRotateUp,0));

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
		m_direction = kRight;
		m_pos = VAdd(m_pos, VGet(kSpped, 
			0.0f,
			0.0f));

		MV1SetRotationXYZ(m_handle, VGet(0, kRotateRight,0));

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
		m_direction = kLeft;
		m_pos = VAdd(m_pos, VGet(-kSpped,
			0.0f,
			0.0f));

		MV1SetRotationXYZ(m_handle, VGet(0,kRotateLeft, 0));

		m_attackCollision.SetCenter(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}
	if (Pad::IsRelase(PAD_INPUT_DOWN))
	{
		m_state = kWait;	
	}

	//�R���g���[���[��X�{�^��
	if (Pad::IsTrigger(PAD_INPUT_C))
	{
		m_isAttack = true;
		Attack();
		m_countXButton++;
		m_state = kAttack;
	}
	if (Pad::IsRelase(PAD_INPUT_C))
	{
		m_state = kWait;
	}

	//�R���g���[���[��A�{�^��
	if (Pad::IsTrigger(PAD_INPUT_A))
	{
		m_countAButton++;
		m_state = kAerialSlash;
	}

	if (Pad::IsTrigger(PAD_INPUT_B))
	{
		m_state = kAttackMagic;
	}
	
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
		if (m_isAttacking !=m_isAttack)
		{
			m_isAttacking = m_isAttack;
			if(m_isAttacking && m_countXButton == 1)
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
	if (m_state == kAerialSlash)
	{
		m_pos = VAdd(m_pos, VGet(0, kJumpPower, 0));

		if (m_countAButton == 2)
		{
			ChangeAnim(kAerialSlashAnimIndex);
			m_countAButton = 0;
		}
	}
	if (m_state == kAttackMagic)
	{
		ChangeAnim(kAttackMagicAnimIndex);
	}
}

bool Knight::UpdateAnim(int attachNo)
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

	if (m_direction == kRight)
	{
		m_attackCollision.SetCenter(m_pos.x + kAttackLeftXAdj, m_pos.y, m_pos.z-kAttackLeftZAdj , kModelWidth, kModelHeight, kModelDepth);
	}
	if (m_direction == kLeft)
	{
		m_attackCollision.SetCenter(m_pos.x - kAttackRightXAdj, m_pos.y, m_pos.z-kAttackRightZAdj , kModelWidth, kModelHeight, kModelDepth);
	}
	if (m_direction == kDown)
	{
		m_attackCollision.SetCenter(m_pos.x-kAttackDownXAdj, m_pos.y, m_pos.z - kAttackDownZAdj, kModelWidth, kModelHeight, kModelDepth);
	}
	if (m_direction == kUp)
	{
		m_attackCollision.SetCenter(m_pos.x - kAttackUpXAdj, m_pos.y, m_pos.z + kAttackUpZAdj, kModelWidth, kModelHeight, kModelDepth);
	}
}

//HP�o�[�̕`��
void Knight::DrawHPBar()
{

	// HP �̒l�𕶎���ƃo�[�ŕ\��
	DrawFormatString(50, 50, GetColor(255, 255, 0), "%4d", m_hp);
	// HP �̒l���̑傫�������l�p�Ɏ��܂�悤�ɒl��傫�����܂�
	DrawFillBox(200, 50, 200 + m_hp	 * kDrawSize, 66, GetColor(77, 181, 106));
	DrawLineBox(200, 50, 450 + kHpMax, 66, GetColor(77, 181, 0));
}

//void Knight::AttackMagic()
//{
//}

//�G�Ɠ����������̏���
void Knight::HitBody()
{	
	m_hp -= 5;

	PlaySoundMem(m_damageSE, DX_PLAYTYPE_BACK);

	if (m_direction == kUp)
	{
		m_pos.z += 250.0f;
	}
	if(m_direction == kDown)
	{
		m_pos.z -= 250.0f;
	}
	if (m_direction == kRight)
	{
		m_pos.x += 250.0f;
	}
	if (m_direction == kLeft)
	{
		m_pos.x -= 250.0f;
	}
	

}

//HP��0�ɂȂ������̏���
void Knight::HpManager()
{
	if (m_hp <= 0)
	{
		//m_isHp0 -= true;
	}
}

void Knight::ConditionCleared(bool clear)
{
	if (clear)
	{
		m_stageClear++;
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

