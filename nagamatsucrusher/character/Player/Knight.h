#pragma once
#include "Rect.h"
class Knight
{
public:
	Knight();
	virtual ~Knight();

	void Init();
	void Update(float cameraAngle);
	void Draw();

	//敵と当たった時の処理
	void HitBody();

	//Hpがになったら負け
	void HpManager();

	//条件をクリアしないと先に進めない
	void ConditionCleared(bool clear);
	
	//Hpが0になったかどうかを与える
	bool GetIsHp0() const noexcept {return m_isHp0;}

	//プレイヤーの座標位置を与える
	VECTOR GetPlayerPos() const noexcept { return m_pos; }

	//プレイヤーの当たり判定を与える
	Rect GetPlayerCollision() const noexcept { return m_playerCollision; }

	//攻撃の当たり判定を与える
	Rect GetAttackCollision() const noexcept { return m_attackCollision; }

	
private:

	//操作
	void Operarion(float cameraAngle);

	//ステージ外に出ないようにする処理
	void StageProcess();

	//どっち向いているのか
	void DirectionFacing();

	//ボタンを押したときのカウントを一定時間過ぎると初期化する処理
	void ButtonCountProcess();

	//アニメーション関連
	void  Animation();

	//アニメーションの進行
	//ループしたかどうかを返す
	bool UpdateAnim(int attachNo);

	//アニメーションの変更
	void ChangeAnim(int animIndex);

	//通常攻撃
	void Attack();

	//HPバー表示
	void DrawHPBar();

	//無敵
	void InvincibleTime();

	//のけ反り
	void LeaningBack();

private:

	//プレイヤーの状態
	enum  state
	{
		kWait,			//待機中
		kMove,			//動いている
		kAttack,		//攻撃中
	};

	//プレイヤーがどちらを向いているか
	enum direction
	{
		kRight,					//右
		kLeft,					//左
		kBefore,				//前
		kBehind,				//後ろ
		kDiagonallyRightFront,	//右斜め前
		kDiagonallyLeftFront,	//左斜め前
		kRightDiagonalBack,		//右斜め後ろ
		kLeftDiagonalBack		//左斜め後ろ

	};

private:

	//HP
	int m_hp;

	//モデルのハンドル
	int m_handle;

	//出血エフェクトのハンドル
	int m_bloodHandle;

	//剣で切った時のSE
	int m_killSE;

	//ダメージを負った時のSE
	int m_damageSE;

	//再生中のエフェクトハンドル
	int m_playingEffectHandle;

	//エフェクトの再生時間
	int m_timer;

	//現在の状態
	int m_state;

	//ノックバック
	int m_leaningBack;

	//向いている方向
	int m_direction;

	//Xボタンを何回押したか
	int m_countXButton;

	//ステージクリア
	int m_stageClear;

	//ボタンのリセット
	int m_count;

	//無敵時間
	int m_invincibleTime;

	//重力
	float m_gravity;

	//アニメーション情報
	int m_currentAnimNo;	//現在のアニメーション
	int m_prevAnimNo;		//変更前のアニメーション
	float m_animBlendRate;	//アニメーション合成割合

	//プレイヤーの向いている方向
	float m_playerAngle;

	//走っているかどうか
	bool m_isRun;

	//走っているかを保持する
	bool m_isMove;

	//攻撃しているかどうか
	bool m_isAttack;
	
	//攻撃しているかを保持する
	bool m_isAttacking;

	//敵とあたったかどうか
	bool m_isHit;

	//Hpが0になったかどうか
	bool m_isHp0;

	//座標
	VECTOR m_pos;

	//移動量
	VECTOR m_move;

	//カメラの回転を方向に変換した値を保存する
	VECTOR m_movementDirection;

	//カメラの前方ベクトル
	VECTOR m_cameraForward;

	//カメラの右方向ベクトル
	VECTOR m_cameraRight;

	//ライトの位置
	VECTOR m_lightPos;

	//カメラのマトリックス
	MATRIX m_rotMatrix;

	// モデルの回転行列を作成
	MATRIX m_modelRotMatrix;

	//当たり判定
	//プレイヤーの当たり判定
	Rect m_playerCollision;

	//攻撃の当たり判定
	Rect m_attackCollision;
};

