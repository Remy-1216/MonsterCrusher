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

void EnemyBase::End()
{
	MV1DeleteModel(m_handle);
}
