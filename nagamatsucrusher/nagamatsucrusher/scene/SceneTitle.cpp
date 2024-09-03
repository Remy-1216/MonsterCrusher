#include "DxLib.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneResult.h"
#include "Pad.h"
#include "Game.h"


namespace
{
	//フォントのサイズ
	constexpr int kFontSize = 32;

	//文字の位置
	constexpr int kFontPosX = 500;
	constexpr int kFontPosY = 500;


	//モデルの初期位置
	constexpr float kPosX =1000.0f;

	constexpr float kPosY = 250.0f;

	constexpr float kPosZ = -40.0f;

	//モデルのサイズ変更
	constexpr float kExpansion = 100.0f;

	//アニメモーションの番号
	//待機モーション
	constexpr int kStandByAnimIndex = 73;

	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 4.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//フェードイン、フェードアウトの数値
	constexpr int kFadeValue = 255;

	//フェード値の増減
	constexpr int kFadeUpDown = 8;
	
}

SceneTitle::SceneTitle():m_animBlendRate(-1), m_currentAnimNo(-1), m_prevAnimNo(-1),m_state(kWait)
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

	//モデルのサイズ調整
	MV1SetScale(m_modelHandle, VGet(kExpansion, kExpansion, kExpansion));

	//アニメーションの初期設定
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, kStandByAnimIndex, -1, true);


	ChangeFont("Makinas-4-Flat");

	SetFontSize(kFontSize);

	m_isSceneEnd = false;	

	m_bgm = LoadSoundMem("data/BGM/start.mp3");

	m_decisionSE = LoadSoundMem("data/SE/decision.mp3");

	m_pos = VGet(kPosX, kPosY, kPosZ);

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
	
	m_state = kWait;

	Animation();

	//モデルの位置更新
	MV1SetPosition(m_modelHandle, m_pos);

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


	return shared_from_this();
}

void SceneTitle::Draw()
{

	DrawGraph(0, 0, m_handle, true);

	MV1DrawModel(m_modelHandle);
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
	if (m_prevAnimNo != -1)
	{
		//test 8フレームで切り替え
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= 1.0f) m_animBlendRate = 1.0f;
		//変更後のアニメーション割合を設定する
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
	//アニメーションが設定されていないので終了
	if (attachNo == -1) return false;

	//アニメーションを進行させる
	float now = MV1GetAttachAnimTime(m_modelHandle, attachNo);

	//アニメーション進める
	now += 0.5f;

	//現在再生中のアニメーションの総カウントを取得する
	float total = MV1GetAttachAnimTotalTime(m_modelHandle, attachNo);
	bool isLoop = false;

	while (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	//進めた時間に設定
	MV1SetAttachAnimTime(m_modelHandle, attachNo, now);
	return isLoop;
}

void SceneTitle::ChangeAnim(int animIndex)
{
	//さらに古いアニメーションがアタッチされている場合はこの時点で削除しておく
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_prevAnimNo);
	}

	//現在再生中の待機アニメーションは変更前のアニメーション扱いに
	m_prevAnimNo = m_currentAnimNo;

	//変更後のアニメーションとして攻撃アニメーションを改めて設定する
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, animIndex, -1, false);

	//切り替えの瞬間は変更前のアニメーションが再生される状態にする
	m_animBlendRate = 0.0f;

	//変更前のアニメーション100%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//変更後のアニメーション0%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);
}
