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
	
	//背景
	int m_handle;

	//ステージ
	int m_stageHandle;

	//BGM
	int m_bgm;

	//ステージの位置
	VECTOR m_pos;

	//カメラの座標
	VECTOR m_cameraPos;
	VECTOR m_cameraTarget;
};

