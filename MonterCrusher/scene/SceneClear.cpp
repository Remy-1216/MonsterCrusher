#include "SceneClear.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include <cmath>
#include "Game.h"
#include "Pad.h"

namespace
{
	//�����̈ʒu
	constexpr int kFontPosX = 750;
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
	constexpr float kPosY =180.0f;
	constexpr float kPosZ = -800.0f;

	//�i�C�g�̍��W
	constexpr float kKnightPosX = 0.0f;
	constexpr float kKnightPosY = 180.0f;
	constexpr float kKnightPosZ = -700.0f;

	//���f���̃T�C�Y�ύX
	constexpr float kExpansion = 150.0f;

	//�A�j���[�V�����ԍ�
	//���S��
	constexpr int kWinAnimIndex = 23;


	//�t�F�[�h�C���A�t�F�[�h�A�E�g�̐��l
	constexpr int kFadeValue = 255;

	//�t�F�[�h�l�̑���
	constexpr int kFadeUpDown = 8;

}
SceneClear::SceneClear()
{
}

SceneClear::~SceneClear()
{
	DeleteGraph(m_handle);

	DeleteSoundMem(m_bgm);

	MV1DeleteModel(m_stageHandle);

	MV1DeleteModel(m_knightHandle);
}

void SceneClear::Init()
{
	//BGM�����[�h
	m_bgm = LoadSoundMem("data/BGM/GameClear.mp3");

	//�w�i�����[�h
	m_handle = LoadGraph("data/Bg/GameClear.png");

	//�X�e�[�W�̃��f�������[�h
	m_stageHandle = MV1LoadModel("data/model/stage/Clear.mv1");

	//�i�C�g�̃��f�������[�h
	m_knightHandle = MV1LoadModel("data/model/player/Knight.mv1");
	 
	//���f���̃T�C�Y�ύX
	MV1SetScale(m_knightHandle, VGet(kExpansion, kExpansion, kExpansion));

	//���f���ɃA�j���[�V�������A�^�b�`����
	m_attachIndex = MV1AttachAnim(m_knightHandle, kWinAnimIndex, -1, false);

	//�A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
	m_totalTime = MV1GetAttachAnimTotalTime(m_knightHandle, m_attachIndex);

	//���s100�`5000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(kMinimumDrawingrange, kMaximumDrawingrange);

	//�X�e�[�W�̍��W�ݒ�
	m_pos = VGet(kPosX, kPosY, kPosZ);

	//�i�C�g�̍��W�ݒ�
	m_knightPos = VGet(kKnightPosX, kKnightPosY, kKnightPosZ);

	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP, true);

	//�Đ����Ԃ̏�����
	m_playTime = 0;

	m_isSceneEnd = false;

	//�t�F�[�h�l�̏����ݒ�
	m_fadeAlpha = kFadeValue;
}

std::shared_ptr<SceneBase> SceneClear::Update()
{
	//�i�C�g�̈ʒu�X�V
	MV1SetPosition(m_knightHandle, m_knightPos);

	//�X�e�[�W�̈ʒu�X�V
	MV1SetPosition(m_stageHandle, m_pos);

	// �J�������W�̓v���C���[���W���班�����ꂽ�Ƃ���
	m_cameraPos = VAdd(m_pos, VGet(0.0f, kCamaraPosY, kCamaraPosZ));

	// �����_�̍��W�̓v���C���[���W�̏�����
	m_cameraTarget = VAdd( m_pos, VGet(0.0f, kCameraTagertY, 0.0f));

	//�J�����̈ʒu�ݒ�
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	// �Đ����Ԃ�i�߂�
	m_playTime += 1.0f;

	// �Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������Đ����Ԃ��O�ɖ߂�
	if (m_playTime >= m_totalTime)
	{
		m_playTime = 0.0f;
	}

	// �Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(m_knightHandle, m_attachIndex, m_playTime);
	
	//�t�F�[�h�֌W
	Fade();

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

void SceneClear::Draw()
{
	//�w�i�̕`��
	DrawGraph(0, 0, m_handle, true);

	//�X�e�[�W�̕`��
	MV1DrawModel(m_stageHandle);

	//�����̕`��
	DrawString(kFontPosX, kFontPosY, "A�{�^���������ă^�C�g����ʂɖ߂�", 0x000000);

	//���f���̕`��
	MV1DrawModel(m_knightHandle);

	if (!m_isSceneEnd)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //�������ŕ\��
		DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(240, 215, 53), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //�s�����ɖ߂��Ă���
	}
	//�Q�[���I�[�o�[���̃t�F�[�h�̕`��
	if (m_isSceneEnd)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //�������ŕ\��
		DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0,0,0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //�s�����ɖ߂��Ă���
	}
	
#ifdef _DEBUG
	DrawFormatString(60, 200, GetColor(255, 255, 255), "player�̍��W(%.2f,%.2f,%.2f)",
		m_knightPos.x, m_knightPos.y, m_knightPos.z);

	DrawFormatString(60, 400, GetColor(255, 255, 255), "�J�����̍��W(%.2f,%.2f,%.2f)",
		m_cameraPos.x, m_cameraPos.y, m_cameraPos.z);

	DrawFormatString(60, 600, GetColor(255, 255, 255), "�X�e�[�W�̍��W(%.2f,%.2f,%.2f)",
		m_pos.x, m_pos.y, m_pos.z);
#endif
}

void SceneClear::End()
{
}

void SceneClear::Fade()
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
