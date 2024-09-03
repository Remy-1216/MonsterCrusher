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
#include "EnemyManager.h"
#include "StageManager.h"

namespace
{
	//操作説明の位置
	constexpr int kOperationX = 1110;
	constexpr int kOperationY = 100;

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

	m_handle = LoadGraph("data/Bg/game.png");

	m_operationHandle = LoadGraph("data/operationinstructions/operation.png");

	m_bgm = LoadSoundMem("data/BGM/GamePlay.mp3");

	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP, true);
}

std::shared_ptr<SceneBase> SceneGame::Update()
{
	m_cameraPos = m_pCamera->GetCameraPos();

	m_pPlayer->Update(m_cameraPos);

	m_playerPos = m_pPlayer->GetPlayerPos();
	
	m_playerCollision = m_pPlayer->GetPlayerCollision();
	
	m_playerAttackCollision = m_pPlayer->GetAttackCollision();
	
	m_isHp0 = m_pPlayer->GetIsHp0();
	
	m_pEnemyManager->Update(m_pPlayer, m_playerPos, m_playerCollision, m_playerAttackCollision);
	
	m_isStageClear = m_pEnemyManager->GetStageClear();

	m_pPlayer->ConditionCleared(m_isStageClear);

	m_pCamera->Update(m_playerPos);
	

#if _DEBUG
	if (Pad::IsTrigger(PAD_INPUT_8))
	{
		//クリアしたので、クリア画面に移行する
		return std::make_shared<SceneClear>();
	}
	if (Pad::IsTrigger(PAD_INPUT_9))
	{
		return std::make_shared<SceneResult>();
	}

#endif

	if (m_isGameClear == true)
	{
		//クリアしたので、クリア画面に移行する
		return std::make_shared<SceneClear>();
	}
	if (m_isHp0 == true)
	{
		return std::make_shared<SceneResult>();
	}

	return shared_from_this();
}

void SceneGame::Draw()
{
	DrawGraph(0, 0, m_handle, true);
	m_pStageManager->Draw();
	m_pPlayer->Draw();
	m_pEnemyManager->Draw();
	m_pCamera->Draw();
	DrawGraph(kOperationX, kOperationY, m_operationHandle, true);
}

void SceneGame::End()
{
	
}

void SceneGame::GameClear()
{
	
}

