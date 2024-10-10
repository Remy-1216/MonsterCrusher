#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "EnemyBase.h"

namespace
{
	//���f���̃T�C�Y�̑傫���̒���
	constexpr float kExpansion = 0.75f;


	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//�m�b�N�o�b�N
	constexpr int kKnockback = 1;

	constexpr int kMaxKnockback =15;

}

EnemyBase::EnemyBase(int ModelHandle)
{
	//���f�������[�h����
	m_handle = ModelHandle;

	//�_���[�W������������SE�̃��[�h
	m_damageSE = LoadSoundMem("data/SE/Cut.mp3");

	//�ŏ��̏�Ԃ͓����Ă���
	m_state = kMove;

	//���f���̑傫����ύX
	MV1SetScale(m_handle, VGet(kExpansion, kExpansion, kExpansion));
}

EnemyBase::~EnemyBase()
{

	MV1DeleteModel(m_damageSE);

	DeleteEffekseerEffect(m_bloodHandle);
}

void EnemyBase::Init()
{
	
}

void EnemyBase::Draw()
{
#if _DEBUG
	m_enemyCollision.Draw(0x000000,true);
#endif
	//�|�W�V�����̐ݒ�
	MV1SetPosition(m_handle, m_pos);

	// �R�c���f���̕`��
	MV1DrawModel(m_handle);
}

void EnemyBase::Knockback(int direction)
{
	//�U�����󂯂Ă���ǂꂾ���ړ����邩
	m_knockback += kKnockback;
	//�����ɂ���ĉ������������ς��
	if (direction == kRight)
	{
		m_pos.z -= m_knockback;
	}
	if (direction == kBehind)
	{
		m_pos.x -= m_knockback;
	}
	if (direction == kBefore)
	{
		m_pos.x += m_knockback;
	}
	if (direction == kLeft)
	{
		m_pos.z += m_knockback;
	}

	if (m_knockback >= kMaxKnockback)
	{
		m_knockback = 0;
	}

}

void EnemyBase::End()
{
	MV1DeleteModel(m_handle);
}
