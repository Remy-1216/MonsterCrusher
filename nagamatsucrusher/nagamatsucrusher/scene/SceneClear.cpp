#include "SceneClear.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include <cmath>
#include "Pad.h"

namespace
{
	//�J�����̕`��͈�
	constexpr float kMinimumDrawingrange = 100.0f;
	constexpr float kMaximumDrawingrange = 5000.0f;

	//�X�e�[�W�̍��W
	constexpr float kPosX = 0.0f;

	constexpr float kPosY =180.0f * DX_PI_F / 180.0f;

	constexpr float kPosZ = -800.0f;
}
SceneClear::SceneClear()
{
}

SceneClear::~SceneClear()
{
	DeleteGraph(m_handle);

	DeleteSoundMem(m_bgm);

	MV1DeleteModel(m_stageHandle);
}

void SceneClear::Init()
{
	m_bgm = LoadSoundMem("data/BGM/GameClear.mp3");

	m_handle = LoadGraph("data/Bg/game.png");

	m_stageHandle = MV1LoadModel("data/model/stage/Clear.mv1");

	//���s100�`5000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(kMinimumDrawingrange, kMaximumDrawingrange);

	m_pos = VGet(kPosX, kPosY, kPosZ);

	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP, true);
}

std::shared_ptr<SceneBase> SceneClear::Update()
{

	// �J�������W�̓v���C���[���W���班�����ꂽ�Ƃ���
	m_cameraPos = VAdd(m_pos, VGet(-300.0f, 100.0f, -850.0f));

	// �����_�̍��W�̓v���C���[���W�̏�����
	m_cameraTarget = VAdd(m_pos, VGet(0.0f, 220.0f, 0.0f));

	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	return shared_from_this();
}

void SceneClear::Draw()
{
	DrawGraph(0, 0, m_handle, true);

	MV1DrawModel(m_stageHandle);
}

void SceneClear::End()
{
}
