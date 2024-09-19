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
	//��������̈ʒu
	constexpr int kOperationX = 1750;
	constexpr int kOperationY = 150;

	//�t�F�[�h�C���A�t�F�[�h�A�E�g�̐��l
	constexpr int kFadeValue = 255;

	//�t�F�[�h�l�̑���
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
		//�J�����A���O�����擾����
		m_cameraAngle = m_pCamera->GetCameraAngle();

		m_pPlayer->Update(m_cameraAngle);

		//�v���C���[�̍��W�ʒu���擾
		m_playerPos = m_pPlayer->GetPlayerPos();

		//�v���C���[�̓����蔻����擾
		m_playerCollision = m_pPlayer->GetPlayerCollision();

		//�v���C���[�̍U���̓����蔻����擾
		m_playerAttackCollision = m_pPlayer->GetAttackCollision();

		//�v���C���[��HP��0�ɂȂ������ǂ������擾
		m_isHp0 = m_pPlayer->GetIsHp0();

		m_pEnemyManager->Update(m_pPlayer, m_playerPos, m_playerCollision, m_playerAttackCollision);

		//�X�e�[�W���Ō�܂ŃN���A������
		m_isClear = m_pEnemyManager->GetClear();

		//�X�e�[�W�̓G��|���������擾
		m_isStageClear = m_pEnemyManager->GetStageClear();

		//�X�e�[�W���N���A�������𔻒f����
		m_pPlayer->ConditionCleared(m_isStageClear);

		m_pCamera->Update(m_playerPos);

	}
	
	//�t�F�[�h�C���A�t�F�[�h�A�E�g���s��
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
		//�v���C���[��HP��0�ɂȂ����̂Ń��U���g��ʂɑJ�ڂ���
		return std::make_shared<SceneResult>();
	}

	return shared_from_this();
}

void SceneGame::Draw()
{
	//�w�i��`�悷��
	DrawGraph(0, 0, m_handle, true);

	//�X�e�[�W��`�悷��
	m_pStageManager->Draw();
	
	//�v���C���[��`�悷��
	m_pPlayer->Draw();
	
	//�G��`�悷��
	m_pEnemyManager->Draw();
	
	//Debug��Ԃ̎������J�����̍��W�ʒu��`�悷��
	m_pCamera->Draw();

	//�����������ʂ̒[�ɕ`�悷��
	DrawGraph(kOperationX, kOperationY, m_operationHandle, true);
	
	if (!m_isSceneEnd)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //�������ŕ\��
		DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //�s�����ɖ߂��Ă���
	}
	//�Q�[���I�[�o�[���̃t�F�[�h�̕`��
	if (m_isHp0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //�������ŕ\��
		DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(157, 9, 12), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //�s�����ɖ߂��Ă���
	}
	//�Q�[���N���A���̃t�F�[�h�̕`��
	if (m_isClear)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //�������ŕ\��
		DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(240, 215, 53), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //�s�����ɖ߂��Ă���
	}

}

void SceneGame::End()
{
	
}

void SceneGame::Fade()
{
	//�t�F�[�h�C���A�t�F�[�h�A�E�g
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

