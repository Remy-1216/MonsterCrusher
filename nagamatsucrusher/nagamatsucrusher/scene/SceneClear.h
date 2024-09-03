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
	
	//�w�i
	int m_handle;

	//�X�e�[�W
	int m_stageHandle;

	//BGM
	int m_bgm;

	//�X�e�[�W�̈ʒu
	VECTOR m_pos;

	//�J�����̍��W
	VECTOR m_cameraPos;
	VECTOR m_cameraTarget;
};

