#pragma once
#include <vector>
#include "SceneBase.h"
#include "DxLib.h"
#include "Rect.h"
class Knight;
class Camera;
class EnemyManager;
class StageManager;
class SceneGame :public SceneBase
{
public:
	SceneGame();
	~SceneGame();

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();
private:

	Knight* m_pPlayer;
	Camera* m_pCamera;
	EnemyManager* m_pEnemyManager;
	StageManager* m_pStageManager;
private:
	//�Ō�̓G��|������
	void GameClear();
private:
	//�w�i�̃n���h��
	int m_handle;

	//BGM
	int m_bgm;

	//�Q�[���N���A�Ȃ̂��̃t���O
	bool m_isGameClear;

	//�v���C���[�ɓ����������̃t���O
	bool m_isHitPlayer;

	//�v���C���[��HP��0�ɂȂ������̃t���O
	bool m_isHp0;

	//�X�e�[�W���N���A������
	bool m_isStageClear;

	//�v���C���[�̍��W
	VECTOR m_playerPos;

	//�J�����̍��W
	VECTOR m_cameraPos;

	//�v���C���[�̓����蔻��
	Rect m_playerCollision;

	//�v���C���[�̍U���̓����蔻��
	Rect m_playerAttackCollision;

};

