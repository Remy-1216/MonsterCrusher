 #include "DxLib.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneResult.h"
#include "Pad.h"
#include "Game.h"


namespace
{
	//フォントのサイズ
	constexpr int kFontSize = 64;

	//文字の位置
	constexpr int kFontPosX = 1000;
	constexpr int kFontPosY = 950;

	//カメラの座標
	constexpr float kCameraPosX = 200.0f;
	constexpr float kCameraPosY = 100.0f;
	constexpr float kCameraPosZ = -350.0f;

	//カメラターゲットの座標
	constexpr  float kCameraTargetPosX = -350.0f;
	constexpr  float kCameraTargetPosY = 120.0f;

	//モデルの初期位置
	constexpr float kPosX =1050.0f;
	constexpr float kPosY = 250.0f;
	constexpr float kPosZ = -400.0f;

	//エネミーの初期座標
	constexpr float kEnemyPosY = 150.0f;

	//ハチの初期位置
	constexpr float kBeePosX = 500.0f;
	constexpr float kBeePosZ = -750.0f;

	//スライムの初期位置
	constexpr float kSlimePosX = 300.0f;
	constexpr float kSlimePosZ = -550.0f;

	//モデル回転
	constexpr float kRotateY = 270 * DX_PI_F / 180;

	//モデルのサイズ変更
	constexpr float kExpansion = 50.0f;

	//アニメモーションの番号
	//待機モーション
	constexpr int kStandByAnimIndex = 73;

	//ハチのアニメーション番号
	// ふわふわ浮いているモーション
	constexpr int kBeeAnimIndex = 2;

	
	//スライムのアニメーション番号
	//跳ねてるモーション
	constexpr int kSlimeAnimIndex = 6;

	//フェードイン、フェードアウトの数値
	constexpr int kFadeValue = 255;

	//フェード値の増減
	constexpr int kFadeUpDown = 8;

	//再生する時間
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
	//背景のロード
	m_handle = LoadGraph("data/Bg/title.png");

	//モデルのロード
	m_modelHandle = MV1LoadModel("data/model/player/knight.mv1");

	//ハチのロード
	m_beeHandle = MV1LoadModel("data/model/enemy/Bee.mv1");

	//スライムのロード
	m_slimeHandle = MV1LoadModel("data/model/enemy/Slime.mv1");

	//BGMのロード
	m_bgm = LoadSoundMem("data/BGM/start.mp3");

	//決定音のロード
	m_decisionSE = LoadSoundMem("data/SE/decision.mp3");

	//モデルのサイズ調整
	MV1SetScale(m_modelHandle, VGet(kExpansion, kExpansion, kExpansion));


	//ナイトにアニメーションをアタッチする
	m_attachIndex = MV1AttachAnim(m_modelHandle, kStandByAnimIndex, -1, false);

	//ハチにアニメーションをアタッチする
	m_beeAttachIndex = MV1AttachAnim(m_beeHandle, kBeeAnimIndex, -1, false);

	//スライムにアニメーションをアタッチする
	m_slimeAttachIndex = MV1AttachAnim(m_slimeHandle, kSlimeAnimIndex, -1, false);

	//アタッチしたアニメーションの総再生時間を取得する
	m_totalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_attachIndex);

	//ハチのアタッチしたアニメーションの総再生時間を取得する
	m_beeTotalTime = MV1GetAttachAnimTotalTime(m_beeHandle, m_beeAttachIndex);

	//スライムのアタッチしたアニメーションの総再生時間を取得する
	m_slimeTotalTime = MV1GetAttachAnimTotalTime(m_slimeHandle, m_slimeAttachIndex);

	//ナイトの座標設定
	m_pos = VGet(kPosX, kPosY, kPosZ);

	//ハチの座標位置
	m_beePos = VGet(kBeePosX, kEnemyPosY, kBeePosZ);

	//スライムの座標位置
	m_slimePos = VGet(kSlimePosX, kEnemyPosY, kSlimePosZ);

	//カメラ座標
	m_cameraPos = VAdd(m_pos, VGet(kCameraPosX, kCameraPosY, kCameraPosZ));

	// 注視点
	m_cameraTarget = VAdd(m_pos, VGet(kCameraTargetPosX, kCameraTargetPosY, 0.0f));

	//ハチの回転
	MV1SetRotationXYZ(m_beeHandle, VGet(0.0f, kRotateY, 0.0f));

	//スライムの回転
	MV1SetRotationXYZ(m_slimeHandle, VGet(0.0f, kRotateY, 0.0f));
	
	//フェード値の初期設定
	m_fadeAlpha = kFadeValue;

	//フォント設定
	ChangeFont("Makinas-4-Flat");

	//フォントのサイズ設定
	SetFontSize(kFontSize);

	m_isSceneEnd = false;	

	//BGMの設定
	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP, true);

	
}

std::shared_ptr<SceneBase> SceneTitle::Update()
{
	//Aボタンを押すと移行する
	if (Pad::IsTrigger(PAD_INPUT_A))
	{
		m_isSceneEnd = true;

		PlaySoundMem(m_decisionSE, DX_PLAYTYPE_BACK);
	}

	if (m_isSceneEnd && m_fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneGame>();
	}

	//ナイトの位置更新
	MV1SetPosition(m_modelHandle, m_pos);
	
	//ハチの位置更新
	MV1SetPosition(m_beeHandle, m_beePos);	
	
	//スライムの位置更新
	MV1SetPosition(m_slimeHandle, m_slimePos);

	//アニメーション
	Animation();
	
	//カメラの位置設定
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	//フレームイン、アウト
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


	//モデルの座標を設定
	MV1SetPosition(m_modelHandle, m_pos);

	//カメラの位置設定
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	return shared_from_this();
}

void SceneTitle::Draw()
{
	//背景の描画
	DrawGraph(0, 0, m_handle, true);

	//ナイトの描画
	MV1DrawModel(m_modelHandle);
	
	//ハチの描画
	MV1DrawModel(m_beeHandle);
	
	//スライムの描画
	MV1DrawModel(m_slimeHandle);
	
	//文字の描画
	DrawString(kFontPosX,kFontPosY,"Aボタンを押してスタート", 0x000000);


	//フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
	DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく


#ifdef _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);

#endif
}

void SceneTitle::End()
{
}

void SceneTitle::Animation()
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
	MV1SetAttachAnimTime(m_modelHandle, m_attachIndex, m_playTime);

	//ハチの再生時間をセットする
	MV1SetAttachAnimTime(m_beeHandle, m_beeAttachIndex, m_beePlayTime);

	//スライムの再生時間をセットする
	MV1SetAttachAnimTime(m_slimeHandle, m_slimeAttachIndex, m_slimePlayTime);

}

