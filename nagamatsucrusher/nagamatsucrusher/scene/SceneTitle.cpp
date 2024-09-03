#include "DxLib.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneResult.h"
#include "Pad.h"


namespace
{
	//�t�H���g�̃T�C�Y
	constexpr int kFontSize = 32;

	//�����̈ʒu
	constexpr int kFontPosX = 500;
	constexpr int kFontPosY = 500;


	//���f���̏����ʒu
	constexpr float kPosX =1000.0f;

	constexpr float kPosY = 250.0f;

	constexpr float kPosZ = -40.0f;

	//���f���̃T�C�Y�ύX
	constexpr float kExpansion = 100.0f;

	//�A�j�����[�V�����̔ԍ�
	//�ҋ@���[�V����
	constexpr int kStandByAnimIndex = 73;

	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeFrame = 4.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;
	
}

SceneTitle::SceneTitle():m_animBlendRate(-1), m_currentAnimNo(-1), m_prevAnimNo(-1),m_state(kWait)
{
	
}

SceneTitle::~SceneTitle()
{
	DeleteGraph(m_handle);

	DeleteSoundMem(m_bgm);

	MV1DeleteModel(m_modelHandle);
}

void SceneTitle::Init()
{
	//�w�i�̃��[�h
	m_handle = LoadGraph("data/Bg/title.png");

	//���f���̃��[�h
	m_modelHandle = MV1LoadModel("data/model/player/knight.mv1");

	//���f���̃T�C�Y����
	MV1SetScale(m_modelHandle, VGet(kExpansion, kExpansion, kExpansion));

	//�A�j���[�V�����̏����ݒ�
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, kStandByAnimIndex, -1, true);


	ChangeFont("Makinas-4-Flat");

	SetFontSize(kFontSize);


	m_bgm = LoadSoundMem("data/BGM/start.mp3");

	m_pos = VGet(kPosX, kPosY, kPosZ);

	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP, true);

	
}

std::shared_ptr<SceneBase> SceneTitle::Update()
{

	//���̏ꏊ��A�{�^���������ƈڍs����
	if (Pad::IsTrigger(PAD_INPUT_A))
	{
		return std::make_shared<SceneGame>();
	}
	
	m_state = kWait;

	Animation();

	//���f���̈ʒu�X�V
	MV1SetPosition(m_modelHandle, m_pos);

	


	return shared_from_this();
}

void SceneTitle::Draw()
{

	DrawGraph(0, 0, m_handle, true);

	MV1DrawModel(m_modelHandle);
	DrawString(kFontPosX,kFontPosY,"A�{�^���������ăX�^�[�g", 0x000000);


#ifdef _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);

#endif
}

void SceneTitle::End()
{
}

void SceneTitle::Animation()
{
	if (m_prevAnimNo != -1)
	{
		//test 8�t���[���Ő؂�ւ�
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= 1.0f) m_animBlendRate = 1.0f;
		//�ύX��̃A�j���[�V����������ݒ肷��
		MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);
	}
	bool isLoop = UpdateAnim(m_currentAnimNo);
	UpdateAnim(m_prevAnimNo);

	if (m_state == kWait)
	{
		if (isLoop)
		{
			ChangeAnim(kStandByAnimIndex);
		}
	}
}

bool SceneTitle::UpdateAnim(int attachNo)
{
	//�A�j���[�V�������ݒ肳��Ă��Ȃ��̂ŏI��
	if (attachNo == -1) return false;

	//�A�j���[�V������i�s������
	float now = MV1GetAttachAnimTime(m_modelHandle, attachNo);

	//�A�j���[�V�����i�߂�
	now += 0.5f;

	//���ݍĐ����̃A�j���[�V�����̑��J�E���g���擾����
	float total = MV1GetAttachAnimTotalTime(m_modelHandle, attachNo);
	bool isLoop = false;

	while (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	//�i�߂����Ԃɐݒ�
	MV1SetAttachAnimTime(m_modelHandle, attachNo, now);
	return isLoop;
}

void SceneTitle::ChangeAnim(int animIndex)
{
	//����ɌÂ��A�j���[�V�������A�^�b�`����Ă���ꍇ�͂��̎��_�ō폜���Ă���
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_prevAnimNo);
	}

	//���ݍĐ����̑ҋ@�A�j���[�V�����͕ύX�O�̃A�j���[�V����������
	m_prevAnimNo = m_currentAnimNo;

	//�ύX��̃A�j���[�V�����Ƃ��čU���A�j���[�V���������߂Đݒ肷��
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, animIndex, -1, false);

	//�؂�ւ��̏u�Ԃ͕ύX�O�̃A�j���[�V�������Đ�������Ԃɂ���
	m_animBlendRate = 0.0f;

	//�ύX�O�̃A�j���[�V����100%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//�ύX��̃A�j���[�V����0%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);
}
