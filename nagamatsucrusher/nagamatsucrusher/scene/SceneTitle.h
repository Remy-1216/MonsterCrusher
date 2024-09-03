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
	void  Animation();

	//アニメーションの進行
	//ループしたかどうかを返す
	bool UpdateAnim(int attachNo);

	//アニメーションの変更
	void ChangeAnim(int animIndex);
private:
	enum state
	{
		kWait,			//待機中
	};
private:

	//アニメーション情報
	int m_currentAnimNo;	//現在のアニメーション
	int m_prevAnimNo;		//変更前のアニメーション
	float m_animBlendRate;	//アニメーション合成割合

	//状態
	int m_state;

	//ハンドル
	int m_handle;

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
};

