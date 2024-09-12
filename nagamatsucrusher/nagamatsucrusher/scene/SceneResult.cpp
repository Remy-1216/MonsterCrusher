#include "DxLib.h"
#include "SceneBase.h"
#include "SceneResult.h"
#include "Pad.h"
#include "SceneTitle.h"
#include "Game.h"

namespace
{
	//�����̈ʒu
	constexpr int kFontPosX = 1000;
	constexpr int kFontPosY = 950;

	//�J�����̕`��͈�
	constexpr float kMinimumDrawingrange = 100.0f;
	constexpr float kMaximumDrawingrange = 5000.0f;

	//�J�����̍��W
	constexpr float kCamaraPosY = 250.0f;
	constexpr float kCamaraPosZ = -1000.0f;

	//�J�����^�[�Q�b�g�̍��W
	constexpr float kCameraTagertY = 350.0f;

	//�X�e�[�W�̍��W
	constexpr float kPosX = 0.0f;
	constexpr float kPosY = 180.0f;
	constexpr float kPosZ = -800.0f;

	//�i�C�g�̍��W
	constexpr float kKnightPosX = 0.0f;
	constexpr float kKnightPosY = 180.0f;
	constexpr float kKnightPosZ = -700.0f;

	//�n�`�̍��W
	constexpr float kBeePosX = 300.0f;
	constexpr float kBeePosY = 180.0f;
	constexpr float kBeePosZ = -600.0f;

	//�X���C���̍��W
	constexpr float kSlimePosX = -150.0f;
	constexpr float kSlimePosY = 180.0f;
	constexpr float kSkimePosZ = -600.0f;

	//���f���̃T�C�Y�ύX
	constexpr float kExpansion = 150.0f;

	//�n�`�̃��f����]
	constexpr float kBeeRotateY = 45 * DX_PI_F / 180;

	//�X���C���̃��f����]
	constexpr float kSlimeRotateY = 315 * DX_PI_F / 180;

	//�A�j���[�V�����ԍ�
	//���S��
	constexpr int kDieAnimIndex = 29;

	//�n�`�̃A�j���[�V�����ԍ�
	//�U�����[�V����
	constexpr int kBeeAnimIndex = 0;

	//�X���C���̃A�j���[�V�����ԍ�
	//�U�����[�V����
	constexpr int kSlimeAnimIndex = 0;

	//�Đ����鎞��
	constexpr float kPlayTime = 0.5f;

	//�t�F�[�h�C���A�t�F�[�h�A�E�g�̐��l
	constexpr int kFadeValue = 255;

	//�t�F�[�h�l�̑���
	constexpr int kFadeUpDown = 8;
}

SceneResult::SceneResult()
{
}

SceneResult::~SceneResult()
{
	DeleteSoundMem(m_bgm);
}

void SceneResult::Init()
{
	//�w�i�̃��[�h
	m_handle = LoadGraph("data/Bg/GameOver.png");

	//BGM�����[�h
	m_bgm = LoadSoundMem("data/BGM/GameOver.mp3");

	//�X�e�[�W�̃��f�������[�h
	m_stageHandle = MV1LoadModel("data/model/stage/result.mv1");

	//�i�C�g�̃��f�������[�h
	m_knightHandle = MV1LoadModel("data/model/player/Knight.mv1");

	//�n�`�̃��[�h
	m_beeHandle = MV1LoadModel("data/model/enemy/Bee.mv1");

	//�X���C���̃��[�h
	m_slimeHandle = MV1LoadModel("data/model/enemy/Slime.mv1");

	//���f���̃T�C�Y�ύX
	MV1SetScale(m_knightHandle, VGet(kExpansion, kExpansion, kExpansion));


	//�i�C�g�ɃA�j���[�V�������A�^�b�`����
	m_attachIndex = MV1AttachAnim(m_knightHandle, kDieAnimIndex, -1, false);

	//�n�`�ɃA�j���[�V�������A�^�b�`����
	m_beeAttachIndex = MV1AttachAnim(m_beeHandle, kBeeAnimIndex, -1, false);

	//�X���C���ɃA�j���[�V�������A�^�b�`����
	m_slimeAttachIndex = MV1AttachAnim(m_slimeHandle, kSlimeAnimIndex, -1, false);

	//�A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
	m_totalTime = MV1GetAttachAnimTotalTime(m_knightHandle, m_attachIndex);

	//�n�`�̃A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
	m_beeTotalTime = MV1GetAttachAnimTotalTime(m_beeHandle, m_beeAttachIndex);

	//�X���C���̃A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
	m_slimeTotalTime = MV1GetAttachAnimTotalTime(m_slimeHandle, m_slimeAttachIndex);

	//�n�`�̉�]
	MV1SetRotationXYZ(m_beeHandle, VGet(0.0f, kBeeRotateY, 0.0f));

	//�X���C���̉�]
	MV1SetRotationXYZ(m_slimeHandle, VGet(0.0f, kSlimeRotateY, 0.0f));

	//�X�e�[�W�̍��W�ݒ�
	m_pos = VGet(kPosX, kPosY, kPosZ);

	//�i�C�g�̍��W�ݒ�
	m_knightPos = VGet(kKnightPosX, kKnightPosY, kKnightPosZ);

	//�n�`�̍��W�ݒ�
	m_beePos = VGet(kBeePosX, kBeePosY, kBeePosZ);

	//�X���C���̍��W�ݒ�
	m_slimePos = VGet(kSlimePosX, kSlimePosY, kSkimePosZ);

	//������
	m_playTime = 0.0f;

	m_beePlayTime = 0.0f;
	
	m_slimePlayTime = 0.0f;

	m_isSceneEnd = false;

	//�t�F�[�h�l�̏����ݒ�
	m_fadeAlpha = kFadeValue;

	//BGM�̍Đ�
	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP, true);
}

std::shared_ptr<SceneBase> SceneResult::Update()
{
	
	//�i�C�g�̈ʒu�X�V
	MV1SetPosition(m_knightHandle, m_knightPos);

	//�X�e�[�W�̈ʒu�X�V
	MV1SetPosition(m_stageHandle, m_pos);

	//�n�`�̈ʒu�X�V
	MV1SetPosition(m_beeHandle, m_beePos);	
	
	//�X���C���̈ʒu�X�V
	MV1SetPosition(m_slimeHandle, m_slimePos);

	// �J�������W�̓v���C���[���W���班�����ꂽ�Ƃ���
	m_cameraPos = VAdd(m_pos, VGet(0.0f, kCamaraPosY, kCamaraPosZ));

	// �����_�̍��W�̓v���C���[���W�̏�����
	m_cameraTarget = VAdd(m_pos, VGet(0.0f, kCameraTagertY, 0.0f));

	//�A�j���[�V����
	Animation();

	//�t�F�[�h�֌W
	Fade();

	//�J�����̈ʒu�ݒ�
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);


	//A�{�^���������ƈڍs����
	if (Pad::IsTrigger(PAD_INPUT_A))
	{
		m_isSceneEnd = true;
	}
	if (m_isSceneEnd && m_fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneTitle>();
	}

	return shared_from_this();
}

void SceneResult::Draw()
{
	//�w�i�̕`��
	DrawGraph(0, 0, m_handle, true);

	//�X�e�[�W�̕`��
	MV1DrawModel(m_stageHandle);

	//�����̕`��
	DrawString(kFontPosX, kFontPosY, "A�{�^���������ă^�C�g����ʂɖ߂�", 0x000000);

	//�i�C�g�̕`��
	MV1DrawModel(m_knightHandle);

	//�n�`�̕`��
	MV1DrawModel(m_beeHandle);

	//�X���C���̕`��
	MV1DrawModel(m_slimeHandle);

	if (!m_isSceneEnd)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //�������ŕ\��
		DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(157, 9, 12), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //�s�����ɖ߂��Ă���
	}
	//�Q�[���I�[�o�[���̃t�F�[�h�̕`��
	if (m_isSceneEnd)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //�������ŕ\��
		DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0,0,0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //�s�����ɖ߂��Ă���
	}
}

void SceneResult::End()
{
}

void SceneResult::Animation()
{
	// �i�C�g�̍Đ����Ԃ�i�߂�
	m_playTime += kPlayTime;

	//�n�`�̍Đ����Ԃ�i�߂�
	m_beePlayTime += kPlayTime;

	//�X���C���̍Đ����Ԃ�i�߂�
	m_slimePlayTime += kPlayTime;


	// �Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������Đ����Ԃ��O�ɖ߂�
	if (m_playTime >= m_totalTime)
	{
		m_playTime = 0.0f;
	}


	if (m_beePlayTime >= m_beeTotalTime)
	{
		m_beePlayTime = 0.0f;
	}

	if (m_slimePlayTime >= m_slimeTotalTime)
	{
		m_slimePlayTime = 0.0f;
	}

	//�i�C�g�̍Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(m_knightHandle, m_attachIndex, m_playTime);

	//�n�`�̍Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(m_beeHandle, m_beeAttachIndex, m_beePlayTime);

	//�X���C���̍Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(m_slimeHandle, m_slimeAttachIndex, m_slimePlayTime);

}

void SceneResult::Fade()
{
	//�t�F�[�h�C���A�t�F�[�h�A�E�g
	if (m_isSceneEnd)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha > kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
	else
	{
		m_fadeAlpha -= kFadeUpDown;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}
}
