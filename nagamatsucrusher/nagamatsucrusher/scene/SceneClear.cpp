#include "SceneClear.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include <cmath>
#include "Pad.h"

namespace
{
	//カメラの描画範囲
	constexpr float kMinimumDrawingrange = 100.0f;
	constexpr float kMaximumDrawingrange = 5000.0f;

	//ステージの座標
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

	//奥行100～5000までをカメラの描画範囲とする
	SetCameraNearFar(kMinimumDrawingrange, kMaximumDrawingrange);

	m_pos = VGet(kPosX, kPosY, kPosZ);

	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP, true);
}

std::shared_ptr<SceneBase> SceneClear::Update()
{

	// カメラ座標はプレイヤー座標から少し離れたところ
	m_cameraPos = VAdd(m_pos, VGet(-300.0f, 100.0f, -850.0f));

	// 注視点の座標はプレイヤー座標の少し上
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
