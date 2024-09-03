#pragma once
#include "Rect.h"
class Knight
{
public:
	Knight();
	virtual ~Knight();

	void Init();
	void Update(VECTOR cameraPos);
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
	void Operarion(VECTOR cameraPos);
	//ジャンプ時の処理
	void JumpProcess();

	//ステージ外に出ないようにする処理
	void StageProcess();

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

	//魔法攻撃
	//void AttackMagic();

private:

	//プレイヤーの状態
	enum  state
	{
		kWait,			//待機中
		kMove,			//動いている
		kAttack,		//攻撃中
		kAerialSlash,	//空中斬り中
		kAttackMagic	//魔法攻撃中
	};

	//プレイヤーがどちらを向いているか
	enum direction
	{
		kRight,
		kLeft,
		kUp,
		kDown
	};

private:

	//HP
	int m_hp;

	//モデルのハンドル
	int m_handle;

	//剣で切った時のSE
	int m_killSE;

	//ダメージを負った時のSE
	int m_damageSE;

	//状態
	int m_state;

	//向いている方向
	int m_direction;

	//Aボタンを何回押したか
	int m_countAButton;

	//Xボタンを何回押したか
	int m_countXButton;

	//ステージクリア
	int m_stageClear;

	//重力
	float m_gravity;

	//ボタンのリセット
	int m_count;


	//アニメーション情報
	int m_currentAnimNo;	//現在のアニメーション
	int m_prevAnimNo;		//変更前のアニメーション
	float m_animBlendRate;	//アニメーション合成割合

	//動いているかを保持する
	bool m_isMove;

	//攻撃しているかを保持する
	bool m_isAttacking;

	//走っているかどうか
	bool m_isRun;

	//攻撃しているかどうか
	bool m_isAttack;

	//ジャンプしているかどうか
	bool m_isJump;

	//Hpが0になったかどうか
	bool m_isHp0;

	//座標
	VECTOR m_pos;

	//カメラの位置
	VECTOR m_cameraPos;

	//ライトの位置
	VECTOR m_lightPos;

	//当たり判定
	//プレイヤーの当たり判定
	Rect m_playerCollision;

	//攻撃の当たり判定
	Rect m_attackCollision;
};

