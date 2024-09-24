 #include "DxLib.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneResult.h"
#include "Pad.h"
#include "Game.h"


namespace
{
	//�t�H���g�̃T�C�Y
	constexpr int kFontSize = 64;

	//�����̈ʒu
	constexpr int kFontPosX = 1000;
	constexpr int kFontPosY = 950;

	//�J�����̍��W
	constexpr float kCameraPosX = 200.0f;
	constexpr float kCameraPosY = 100.0f;
	constexpr float kCameraPosZ = -350.0f;

	//�J�����^�[�Q�b�g�̍��W
	constexpr  float kCameraTargetPosX = -350.0f;
	constexpr  float kCameraTargetPosY = 120.0f;

	//���f���̏����ʒu
	constexpr float kPosX =1050.0f;
	constexpr float kPosY = 250.0f;
	constexpr float kPosZ = -400.0f;

	//�G�l�~�[�̏������W
	constexpr float kEnemyPosY = 150.0f;

	//�n�`�̏����ʒu
	constexpr float kBeePosX = 500.0f;
	constexpr float kBeePosZ = -750.0f;

	//�X���C���̏����ʒu
	constexpr float kSlimePosX = 300.0f;
	constexpr float kSlimePosZ = -550.0f;

	//���f����]
	constexpr float kRotateY = 270 * DX_PI_F / 180;

	//���f���̃T�C�Y�ύX
	constexpr float kExpansion = 50.0f;

	//�A�j�����[�V�����̔ԍ�
	//�ҋ@���[�V����
	constexpr int kStandByAnimIndex = 73;

	//�n�`�̃A�j���[�V�����ԍ�
	// �ӂ�ӂ핂���Ă��郂�[�V����
	constexpr int kBeeAnimIndex = 2;

	
	//�X���C���̃A�j���[�V�����ԍ�
	//���˂Ă郂�[�V����
	constexpr int kSlimeAnimIndex = 6;

	//�t�F�[�h�C���A�t�F�[�h�A�E�g�̐��l
	constexpr int kFadeValue = 255;

	//�t�F�[�h�l�̑���
	constexpr int kFadeUpDown = 8;

	//�Đ����鎞��
	constexpr float kPlayTime = 0.5f;

}

SceneTitle::SceneTitle(): m_playTime(0), m_beePlayTime(0), m_slimePlayTime(0),m_handle(-1)
{
	
}

SceneTitle::~SceneTitle()
{
	DeleteGraph(m_handle);

	DeleteSoundMem(m_bgm);

	DeleteSoundMem(m_decisionSE);

	MV1DeleteModel(m_modelHandle);
}

void SceneTitle::Init()
{
	//�w�i�̃��[�h
	m_handle = LoadGraph("data/Bg/title.png");

	//���f���̃��[�h
	m_modelHandle = MV1LoadModel("data/model/player/knight.mv1");

	//�n�`�̃��[�h
	m_beeHandle = MV1LoadModel("data/model/enemy/Bee.mv1");

	//�X���C���̃��[�h
	m_slimeHandle = MV1LoadModel("data/model/enemy/Slime.mv1");

	//BGM�̃��[�h
	m_bgm = LoadSoundMem("data/BGM/start.mp3");

	//���艹�̃��[�h
	m_decisionSE = LoadSoundMem("data/SE/decision.mp3");

	//���f���̃T�C�Y����
	MV1SetScale(m_modelHandle, VGet(kExpansion, kExpansion, kExpansion));


	//�i�C�g�ɃA�j���[�V�������A�^�b�`����
	m_attachIndex = MV1AttachAnim(m_modelHandle, kStandByAnimIndex, -1, false);

	//�n�`�ɃA�j���[�V�������A�^�b�`����
	m_beeAttachIndex = MV1AttachAnim(m_beeHandle, kBeeAnimIndex, -1, false);

	//�X���C���ɃA�j���[�V�������A�^�b�`����
	m_slimeAttachIndex = MV1AttachAnim(m_slimeHandle, kSlimeAnimIndex, -1, false);

	//�A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
	m_totalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_attachIndex);

	//�n�`�̃A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
	m_beeTotalTime = MV1GetAttachAnimTotalTime(m_beeHandle, m_beeAttachIndex);

	//�X���C���̃A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
	m_slimeTotalTime = MV1GetAttachAnimTotalTime(m_slimeHandle, m_slimeAttachIndex);

	//�i�C�g�̍��W�ݒ�
	m_pos = VGet(kPosX, kPosY, kPosZ);

	//�n�`�̍��W�ʒu
	m_beePos = VGet(kBeePosX, kEnemyPosY, kBeePosZ);

	//�X���C���̍��W�ʒu
	m_slimePos = VGet(kSlimePosX, kEnemyPosY, kSlimePosZ);

	//�J�������W
	m_cameraPos = VAdd(m_pos, VGet(kCameraPosX, kCameraPosY, kCameraPosZ));

	// �����_
	m_cameraTarget = VAdd(m_pos, VGet(kCameraTargetPosX, kCameraTargetPosY, 0.0f));

	//�n�`�̉�]
	MV1SetRotationXYZ(m_beeHandle, VGet(0.0f, kRotateY, 0.0f));

	//�X���C���̉�]
	MV1SetRotationXYZ(m_slimeHandle, VGet(0.0f, kRotateY, 0.0f));
	
	//�t�F�[�h�l�̏����ݒ�
	m_fadeAlpha = kFadeValue;

	//�t�H���g�ݒ�
	ChangeFont("Makinas-4-Flat");

	//�t�H���g�̃T�C�Y�ݒ�
	SetFontSize(kFontSize);

	m_isSceneEnd = false;	

	//BGM�̐ݒ�
	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP, true);

	
}

std::shared_ptr<SceneBase> SceneTitle::Update()
{
	//A�{�^���������ƈڍs����
	if (Pad::IsTrigger(PAD_INPUT_A))
	{
		m_isSceneEnd = true;

		PlaySoundMem(m_decisionSE, DX_PLAYTYPE_BACK);
	}

	if (m_isSceneEnd && m_fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneGame>();
	}

	//�i�C�g�̈ʒu�X�V
	MV1SetPosition(m_modelHandle, m_pos);
	
	//�n�`�̈ʒu�X�V
	MV1SetPosition(m_beeHandle, m_beePos);	
	
	//�X���C���̈ʒu�X�V
	MV1SetPosition(m_slimeHandle, m_slimePos);

	//�A�j���[�V����
	Animation();
	
	//�J�����̈ʒu�ݒ�
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	//�t���[���C���A�A�E�g
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


	//���f���̍��W��ݒ�
	MV1SetPosition(m_modelHandle, m_pos);

	//�J�����̈ʒu�ݒ�
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	return shared_from_this();
}

void SceneTitle::Draw()
{
	//�w�i�̕`��
	DrawGraph(0, 0, m_handle, true);

	//�i�C�g�̕`��
	MV1DrawModel(m_modelHandle);
	
	//�n�`�̕`��
	MV1DrawModel(m_beeHandle);
	
	//�X���C���̕`��
	MV1DrawModel(m_slimeHandle);
	
	//�����̕`��
	DrawString(kFontPosX,kFontPosY,"A�{�^���������ăX�^�[�g", 0x000000);


	//�t�F�[�h�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //�������ŕ\��
	DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //�s�����ɖ߂��Ă���


#ifdef _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);

#endif
}

void SceneTitle::End()
{
}

void SceneTitle::Animation()
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
	MV1SetAttachAnimTime(m_modelHandle, m_attachIndex, m_playTime);

	//�n�`�̍Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(m_beeHandle, m_beeAttachIndex, m_beePlayTime);

	//�X���C���̍Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(m_slimeHandle, m_slimeAttachIndex, m_slimePlayTime);

}

