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

	//フェードイン、フェードアウト
	void Fade();

private:
	//背景
	int m_handle;

	//ナイトのモデル
	int m_knightHandle;

	//ステージ
	int m_stageHandle;

	//BGM
	int m_bgm;

	//再生時間
	int m_playTime;

	//アタッチしたアニメーションの再生時間を保存する
	int m_totalTime;

	//アニメーションナンバー
	int m_attachIndex;

	//フェードイン、アウト
	int m_fadeAlpha;

	//タイトル画面に戻るか
	bool m_isSceneEnd;

	//ステージの位置
	VECTOR m_pos;

	//ナイトの位置
	VECTOR m_knightPos;
	
	//カメラの座標
	VECTOR m_cameraPos;
	VECTOR m_cameraTarget;
};

