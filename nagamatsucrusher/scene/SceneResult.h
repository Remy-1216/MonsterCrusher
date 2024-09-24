#pragma once
#include "SceneBase.h"
class SceneResult :public SceneBase
{
public:
	SceneResult();
	~SceneResult();

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

private:

	//アニメーション
	void Animation();

	//フェードイン、フェードアウト
	void Fade();

private:
	//背景のハンドル
	int m_handle;
	
	//ナイトのモデル
	int m_knightHandle;

	//ハチのハンドル
	int m_beeHandle;

	//スライムのハンドル
	int m_slimeHandle;

	//ステージ
	int m_stageHandle;

	//BGM
	int m_bgm;

	//アタッチしたアニメーションの再生時間を保存する
	int m_totalTime;

	//アタッチしたハチのアニメーションの再生時間を保持する
	int m_beeTotalTime;

	//アタッチしたスライムのアニメーションの再生時間を保持する
	int m_slimeTotalTime;

	//ナイトのアニメーションナンバー
	int m_attachIndex;

	//ハチのアニメーションナンバー
	int m_beeAttachIndex;

	//スライムアニメーションナンバー
	int m_slimeAttachIndex;

	//フェードイン、アウト
	int m_fadeAlpha;

	//ナイトの再生時間
	float m_playTime;

	//ハチの再生時間
	float m_beePlayTime;

	//スライムの再生時間
	float m_slimePlayTime;

	//タイトル画面に戻るか
	bool m_isSceneEnd;
	

	//ステージの位置
	VECTOR m_pos;

	//ナイトの位置
	VECTOR m_knightPos;

	//ハチの座標
	VECTOR m_beePos;

	//スライムの座標
	VECTOR m_slimePos;

	//カメラの座標
	VECTOR m_cameraPos;
	VECTOR m_cameraTarget;

};

