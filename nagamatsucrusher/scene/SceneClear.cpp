#include "SceneClear.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include <cmath>
#include "Game.h"
#include "Pad.h"

namespace
{
	//文字の位置
	constexpr int kFontPosX = 750;
	constexpr int kFontPosY = 950;

	//カメラの描画範囲
	constexpr float kMinimumDrawingrange = 100.0f;
	constexpr float kMaximumDrawingrange = 5000.0f;

	//カメラの座標
	constexpr float kCamaraPosY = 250.0f;
	constexpr float kCamaraPosZ = -1000.0f;

	//カメラターゲットの座標
	constexpr float kCameraTagertY = 350.0f;

	//ステージの座標
	constexpr float kPosX = 0.0f;
	constexpr float kPosY =180.0f;
	constexpr float kPosZ = -800.0f;

	//ナイトの座標
	constexpr float kKnightPosX = 0.0f;
	constexpr float kKnightPosY = 180.0f;
	constexpr float kKnightPosZ = -700.0f;

	//モデルのサイズ変更
	constexpr float kExpansion = 150.0f;

	//アニメーション番号
	//死亡時
	constexpr int kWinAnimIndex = 23;


	//フェードイン、フェードアウトの数値
	constexpr int kFadeValue = 255;

	//フェード値の増減
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
	//BGMをロード
	m_bgm = LoadSoundMem("data/BGM/GameClear.mp3");

	//背景をロード
	m_handle = LoadGraph("data/Bg/GameClear.png");

	//ステージのモデルをロード
	m_stageHandle = MV1LoadModel("data/model/stage/Clear.mv1");

	//ナイトのモデルをロード
	m_knightHandle = MV1LoadModel("data/model/player/Knight.mv1");
	 
	//モデルのサイズ変更
	MV1SetScale(m_knightHandle, VGet(kExpansion, kExpansion, kExpansion));

	//モデルにアニメーションをアタッチする
	m_attachIndex = MV1AttachAnim(m_knightHandle, kWinAnimIndex, -1, false);

	//アタッチしたアニメーションの総再生時間を取得する
	m_totalTime = MV1GetAttachAnimTotalTime(m_knightHandle, m_attachIndex);

	//奥行100～5000までをカメラの描画範囲とする
	SetCameraNearFar(kMinimumDrawingrange, kMaximumDrawingrange);

	//ステージの座標設定
	m_pos = VGet(kPosX, kPosY, kPosZ);

	//ナイトの座標設定
	m_knightPos = VGet(kKnightPosX, kKnightPosY, kKnightPosZ);

	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP, true);

	//再生時間の初期化
	m_playTime = 0;

	m_isSceneEnd = false;

	//フェード値の初期設定
	m_fadeAlpha = kFadeValue;
}

std::shared_ptr<SceneBase> SceneClear::Update()
{
	//ナイトの位置更新
	MV1SetPosition(m_knightHandle, m_knightPos);

	//ステージの位置更新
	MV1SetPosition(m_stageHandle, m_pos);

	// カメラ座標はプレイヤー座標から少し離れたところ
	m_cameraPos = VAdd(m_pos, VGet(0.0f, kCamaraPosY, kCamaraPosZ));

	// 注視点の座標はプレイヤー座標の少し上
	m_cameraTarget = VAdd( m_pos, VGet(0.0f, kCameraTagertY, 0.0f));

	//カメラの位置設定
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	// 再生時間を進める
	m_playTime += 1.0f;

	// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
	if (m_playTime >= m_totalTime)
	{
		m_playTime = 0.0f;
	}

	// 再生時間をセットする
	MV1SetAttachAnimTime(m_knightHandle, m_attachIndex, m_playTime);
	
	//フェード関係
	Fade();

	//Aボタンを押すと移行する
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
	//背景の描画
	DrawGraph(0, 0, m_handle, true);

	//ステージの描画
	MV1DrawModel(m_stageHandle);

	//文字の描画
	DrawString(kFontPosX, kFontPosY, "Aボタンを押してタイトル画面に戻る", 0x000000);

	//モデルの描画
	MV1DrawModel(m_knightHandle);

	if (!m_isSceneEnd)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
		DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(240, 215, 53), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく
	}
	//ゲームオーバー時のフェードの描画
	if (m_isSceneEnd)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
		DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0,0,0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく
	}
	
#ifdef _DEBUG
	DrawFormatString(60, 200, GetColor(255, 255, 255), "playerの座標(%.2f,%.2f,%.2f)",
		m_knightPos.x, m_knightPos.y, m_knightPos.z);

	DrawFormatString(60, 400, GetColor(255, 255, 255), "カメラの座標(%.2f,%.2f,%.2f)",
		m_cameraPos.x, m_cameraPos.y, m_cameraPos.z);

	DrawFormatString(60, 600, GetColor(255, 255, 255), "ステージの座標(%.2f,%.2f,%.2f)",
		m_pos.x, m_pos.y, m_pos.z);
#endif
}

void SceneClear::End()
{
}

void SceneClear::Fade()
{
	//フェードイン、フェードアウト
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
