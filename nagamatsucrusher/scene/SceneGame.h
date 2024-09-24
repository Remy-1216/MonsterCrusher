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

	//�t�F�[�h�C���A�A�E�g
	void Fade();
private:

	//�w�i�̃n���h��
	int m_handle;

	//�������
	int m_operationHandle;

	//BGM
	int m_bgm;

	//�t�F�[�h�C���A�A�E�g
	int m_fadeAlpha;

	//�J�����̃A���O��
	float m_cameraAngle;

	//�V�[���I���t���O
	bool m_isSceneEnd;

	//�Q�[���N���A�Ȃ̂��̃t���O
	bool m_isGameClear;

	//�v���C���[�ɓ����������̃t���O
	bool m_isHitPlayer;

	//�v���C���[��HP��0�ɂȂ������̃t���O
	bool m_isHp0;

	//�X�e�[�W���N���A������
	bool m_isStageClear;

	//�N���A������
	bool m_isClear;

	//�v���C���[�̍��W
	VECTOR m_playerPos;

	//�v���C���[�̓����蔻��
	Rect m_playerCollision;

	//�v���C���[�̍U���̓����蔻��
	Rect m_playerAttackCollision;

};

