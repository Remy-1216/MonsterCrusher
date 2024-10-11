#pragma once
#include "DxLib.h"
#include "SceneBase.h"


class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

private:

	//アニメーション関連
	void Animation();

private:
	//ハンドル
	int m_handle;

	//ハチのハンドル
	int m_beeHandle;

	//スライムのハンドル
	int m_slimeHandle;

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

	//ナイトの再生時間
	float m_playTime;

	//ハチの再生時間
	float m_beePlayTime;

	//スライムの再生時間
	float m_slimePlayTime;

	//フェードイン、アウト
	int m_fadeAlpha;

	//決定音
	int m_decisionSE;

	//モデル
	int m_modelHandle;

	//BGM
	int m_bgm;

	//シーン終了フラグ
	bool m_isSceneEnd;	

	//モデルの座標
	VECTOR m_pos;
	
	//ハチの座標
	VECTOR m_beePos;

	//スライムの座標
	VECTOR m_slimePos;

	//カメラの座標
	VECTOR m_cameraPos;
	VECTOR m_cameraTarget;
};

