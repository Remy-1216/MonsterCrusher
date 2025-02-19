#include "DxLib.h"
#include "SceneBase.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "SceneResult.h"
#include "SceneClear.h"
#include "Timer.h"
#include "Knight.h"
#include "Camera.h"
#include "Pad.h"
#include "Game.h"
#include "EnemyManager.h"
#include "StageManager.h"
#include "EffekseerForDXLib.h"

namespace
{
	//操作説明の位置
	constexpr int kOperationX = 1750;
	constexpr int kOperationY = 150;

	//フェードイン、フェードアウトの数値
	constexpr int kFadeValue = 255;

	//フェード値の増減
	constexpr int kFadeUpDown = 8;

}

SceneGame::SceneGame() :m_isGameClear(false)
{
	

}
SceneGame::~SceneGame()
{
	delete m_pPlayer;
	m_pPlayer = nullptr;

	delete m_pEnemyManager;
	m_pEnemyManager = nullptr;

	delete m_pCamera;
	m_pCamera = nullptr;

	delete m_pStageManager;
	m_pStageManager = nullptr;


	DeleteGraph(m_handle);
	DeleteGraph(m_operationHandle);
	DeleteSoundMem(m_bgm);

}

void SceneGame::Init()
{
	m_pPlayer = new Knight;
	m_pPlayer->Init();
	

	m_pCamera = new Camera;
	m_pCamera->Init();

	m_pEnemyManager = new EnemyManager();
	m_pEnemyManager->Init();

	m_pStageManager = new StageManager();
	m_pStageManager->Init();

	m_fadeAlpha = kFadeValue;

	m_isSceneEnd = false;
	m_isGameClear = false;

	m_isClear = false;
	m_isHp0 = false;

	m_handle = LoadGraph("data/Bg/game.png");

	m_operationHandle = LoadGraph("data/operationinstructions/operation.png");

	m_bgm = LoadSoundMem("data/BGM/GamePlay.mp3");

	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP, true);
}

std::shared_ptr<SceneBase> SceneGame::Update()
{
	if (!m_isClear && !m_isHp0)
	{
		//カメラアングルを取得する
		m_cameraAngle = m_pCamera->GetCameraAngle();

		m_pPlayer->Update(m_cameraAngle);

		//プレイヤーの座標位置を取得
		m_playerPos = m_pPlayer->GetPlayerPos();

		//プレイヤーの当たり判定を取得
		m_playerCollision = m_pPlayer->GetPlayerCollision();

		//プレイヤーの攻撃の当たり判定を取得
		m_playerAttackCollision = m_pPlayer->GetAttackCollision();

		//プレイヤーのHPが0になったかどうかを取得
		m_isHp0 = m_pPlayer->GetIsHp0();

		m_pEnemyManager->Update(m_pPlayer, m_playerPos, m_playerCollision, m_playerAttackCollision);

		//ステージを最後までクリアしたか
		m_isClear = m_pEnemyManager->GetClear();

		//ステージの敵を倒せたかを取得
		m_isStageClear = m_pEnemyManager->GetStageClear();

		//ステージをクリアしたかを判断する
		m_pPlayer->ConditionCleared(m_isStageClear);

		m_pCamera->Update(m_playerPos);

	}
	
	//フェードイン、フェードアウトを行う
	Fade();

#if _DEBUG
	if (Pad::IsTrigger(PAD_INPUT_6))
	{
		m_isClear = true;
	}
	if (Pad::IsTrigger(PAD_INPUT_7))
	{
		m_isHp0 = true;
	}

#endif

	if (m_isClear && m_fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneClear>();
	}

	if (m_isHp0 && m_fadeAlpha >= kFadeValue)
	{
		//プレイヤーのHPが0になったのでリザルト画面に遷移する
		return std::make_shared<SceneResult>();
	}

	return shared_from_this();
}

void SceneGame::Draw()
{
	//背景を描画する
	DrawGraph(0, 0, m_handle, true);

	//ステージを描画する
	m_pStageManager->Draw();
	
	//プレイヤーを描画する
	m_pPlayer->Draw();
	
	//敵を描画する
	m_pEnemyManager->Draw();
	
	//Debug状態の時だけカメラの座標位置を描画する
	m_pCamera->Draw();

	//操作説明を画面の端に描画する
	DrawGraph(kOperationX, kOperationY, m_operationHandle, true);
	
	if (!m_isSceneEnd)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
		DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく
	}
	//ゲームオーバー時のフェードの描画
	if (m_isHp0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
		DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(157, 9, 12), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく
	}
	//ゲームクリア時のフェードの描画
	if (m_isClear)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
		DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(240, 215, 53), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく
	}

}

void SceneGame::End()
{
	
}

void SceneGame::Fade()
{
	//フェードイン、フェードアウト
	if (!m_isSceneEnd)
	{
		m_fadeAlpha -= kFadeUpDown;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
			m_isSceneEnd = true;
		}
	}
	if (m_isClear)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha > kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
	if(m_isHp0)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha > kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
}

