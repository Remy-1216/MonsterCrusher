#pragma once
#include "SceneBase.h"
#include "DxLib.h"
class SceneClear :public SceneBase
{
public:
	SceneClear();
	~SceneClear();

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

private:

	//�t�F�[�h�C���A�t�F�[�h�A�E�g
	void Fade();

private:
	//�w�i
	int m_handle;

	//�i�C�g�̃��f��
	int m_knightHandle;

	//�X�e�[�W
	int m_stageHandle;

	//BGM
	int m_bgm;

	//�Đ�����
	int m_playTime;

	//�A�^�b�`�����A�j���[�V�����̍Đ����Ԃ�ۑ�����
	int m_totalTime;

	//�A�j���[�V�����i���o�[
	int m_attachIndex;

	//�t�F�[�h�C���A�A�E�g
	int m_fadeAlpha;

	//�^�C�g����ʂɖ߂邩
	bool m_isSceneEnd;

	//�X�e�[�W�̈ʒu
	VECTOR m_pos;

	//�i�C�g�̈ʒu
	VECTOR m_knightPos;
	
	//�J�����̍��W
	VECTOR m_cameraPos;
	VECTOR m_cameraTarget;
};

