#include "DxLib.h"
#include "SceneBase.h"
#include "SceneResult.h"
#include "Pad.h"
#include "SceneTitle.h"
#include "Game.h"

namespace
{
	//文字の位置
	constexpr int kFontPosX = 1000;
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
	constexpr float kPosY = 180.0f;
	constexpr float kPosZ = -800.0f;

	//ナイトの座標
	constexpr float kKnightPosX = 0.0f;
	constexpr float kKnightPosY = 180.0f;
	constexpr float kKnightPosZ = -700.0f;

	//ハチの座標
	constexpr float kBeePosX = 300.0f;
	constexpr float kBeePosY = 180.0f;
	constexpr float kBeePosZ = -600.0f;

	//スライムの座標
	constexpr float kSlimePosX = -150.0f;
	constexpr float kSlimePosY = 180.0f;
	constexpr float kSkimePosZ = -600.0f;

	//モデルのサイズ変更
	constexpr float kExpansion = 150.0f;

	//ハチのモデル回転
	constexpr float kBeeRotateY = 45 * DX_PI_F / 180;

	//スライムのモデル回転
	constexpr float kSlimeRotateY = 315 * DX_PI_F / 180;

	//アニメーション番号
	//死亡時
	constexpr int kDieAnimIndex = 29;

	//ハチのアニメーション番号
	//攻撃モーション
	constexpr int kBeeAnimIndex = 0;

	//スライムのアニメーション番号
	//攻撃モーション
	constexpr int kSlimeAnimIndex = 0;

	//再生する時間
	constexpr float kPlayTime = 0.5f;

	//フェードイン、フェードアウトの数値
	constexpr int kFadeValue = 255;

	//フェード値の増減
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
	//背景のロード
	m_handle = LoadGraph("data/Bg/GameOver.png");

	//BGMをロード
	m_bgm = LoadSoundMem("data/BGM/GameOver.mp3");

	//ステージのモデルをロード
	m_stageHandle = MV1LoadModel("data/model/stage/result.mv1");

	//ナイトのモデルをロード
	m_knightHandle = MV1LoadModel("data/model/player/Knight.mv1");

	//ハチのロード
	m_beeHandle = MV1LoadModel("data/model/enemy/Bee.mv1");

	//スライムのロード
	m_slimeHandle = MV1LoadModel("data/model/enemy/Slime.mv1");

	//モデルのサイズ変更
	MV1SetScale(m_knightHandle, VGet(kExpansion, kExpansion, kExpansion));


	//ナイトにアニメーションをアタッチする
	m_attachIndex = MV1AttachAnim(m_knightHandle, kDieAnimIndex, -1, false);

	//ハチにアニメーションをアタッチする
	m_beeAttachIndex = MV1AttachAnim(m_beeHandle, kBeeAnimIndex, -1, false);

	//スライムにアニメーションをアタッチする
	m_slimeAttachIndex = MV1AttachAnim(m_slimeHandle, kSlimeAnimIndex, -1, false);

	//アタッチしたアニメーションの総再生時間を取得する
	m_totalTime = MV1GetAttachAnimTotalTime(m_knightHandle, m_attachIndex);

	//ハチのアタッチしたアニメーションの総再生時間を取得する
	m_beeTotalTime = MV1GetAttachAnimTotalTime(m_beeHandle, m_beeAttachIndex);

	//スライムのアタッチしたアニメーションの総再生時間を取得する
	m_slimeTotalTime = MV1GetAttachAnimTotalTime(m_slimeHandle, m_slimeAttachIndex);

	//ハチの回転
	MV1SetRotationXYZ(m_beeHandle, VGet(0.0f, kBeeRotateY, 0.0f));

	//スライムの回転
	MV1SetRotationXYZ(m_slimeHandle, VGet(0.0f, kSlimeRotateY, 0.0f));

	//ステージの座標設定
	m_pos = VGet(kPosX, kPosY, kPosZ);

	//ナイトの座標設定
	m_knightPos = VGet(kKnightPosX, kKnightPosY, kKnightPosZ);

	//ハチの座標設定
	m_beePos = VGet(kBeePosX, kBeePosY, kBeePosZ);

	//スライムの座標設定
	m_slimePos = VGet(kSlimePosX, kSlimePosY, kSkimePosZ);

	//初期化
	m_playTime = 0.0f;

	m_beePlayTime = 0.0f;
	
	m_slimePlayTime = 0.0f;

	m_isSceneEnd = false;

	//フェード値の初期設定
	m_fadeAlpha = kFadeValue;

	//BGMの再生
	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP, true);
}

std::shared_ptr<SceneBase> SceneResult::Update()
{
	
	//ナイトの位置更新
	MV1SetPosition(m_knightHandle, m_knightPos);

	//ステージの位置更新
	MV1SetPosition(m_stageHandle, m_pos);

	//ハチの位置更新
	MV1SetPosition(m_beeHandle, m_beePos);	
	
	//スライムの位置更新
	MV1SetPosition(m_slimeHandle, m_slimePos);

	// カメラ座標はプレイヤー座標から少し離れたところ
	m_cameraPos = VAdd(m_pos, VGet(0.0f, kCamaraPosY, kCamaraPosZ));

	// 注視点の座標はプレイヤー座標の少し上
	m_cameraTarget = VAdd(m_pos, VGet(0.0f, kCameraTagertY, 0.0f));

	//アニメーション
	Animation();

	//フェード関係
	Fade();

	//カメラの位置設定
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);


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

void SceneResult::Draw()
{
	//背景の描画
	DrawGraph(0, 0, m_handle, true);

	//ステージの描画
	MV1DrawModel(m_stageHandle);

	//文字の描画
	DrawString(kFontPosX, kFontPosY, "Aボタンを押してタイトル画面に戻る", 0x000000);

	//ナイトの描画
	MV1DrawModel(m_knightHandle);

	//ハチの描画
	MV1DrawModel(m_beeHandle);

	//スライムの描画
	MV1DrawModel(m_slimeHandle);

	if (!m_isSceneEnd)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
		DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(157, 9, 12), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく
	}
	//ゲームオーバー時のフェードの描画
	if (m_isSceneEnd)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
		DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0,0,0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく
	}
}

void SceneResult::End()
{
}

void SceneResult::Animation()
{
	// ナイトの再生時間を進める
	m_playTime += kPlayTime;

	//ハチの再生時間を進める
	m_beePlayTime += kPlayTime;

	//スライムの再生時間を進める
	m_slimePlayTime += kPlayTime;


	// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
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

	//ナイトの再生時間をセットする
	MV1SetAttachAnimTime(m_knightHandle, m_attachIndex, m_playTime);

	//ハチの再生時間をセットする
	MV1SetAttachAnimTime(m_beeHandle, m_beeAttachIndex, m_beePlayTime);

	//スライムの再生時間をセットする
	MV1SetAttachAnimTime(m_slimeHandle, m_slimeAttachIndex, m_slimePlayTime);

}

void SceneResult::Fade()
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
