#pragma once
#include "DxLib.h"
#include "Rect.h"
class Knight;
class EnemyBase
{
public:
	EnemyBase(int ModelHandle);
	virtual ~EnemyBase();

	void Init();
	
	virtual void Update(Knight* knight, VECTOR playerPos) abstract;
	
	void Draw();	//ループしたかどうかを返す

	//プレイヤーの位置によって生成する位置を変える
	virtual void SetPosX(VECTOR playerPos)abstract;

	void End();

	//座標の設定
	void SetPos(VECTOR mpos) { m_pos = mpos; }
protected:

	//エネミーの状態
	enum  state
	{
		kMove,			//動いている
		kAttack,		//攻撃中
		kDamage,		//ダメージ受けている
		kDeath			//死んだ
	};

	//エネミーがどちらを向いているか
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
protected:

	//状態
	int m_state;

	int	m_animFrame;

	//HP
	int m_hp;

	//攻撃力
	int m_attackPower;

	//モデルのハンドル
	int m_handle;

	//攻撃を受けた時のSE
	int m_damageSE;

	//出血エフェクトのハンドル
	int m_bloodHandle;

	//エフェクトの再生時間
	int m_timer;

	//再生中のエフェクトハンドル
	int m_playingEffectHandle;

	//プレイヤーの人数
	int m_playerNum;	
	
	//無敵時間
	int m_invincibleTime;

	//攻撃に当たったかどうか
	bool m_isHitAttack;

	//プレイヤーに当たったかどうか
	bool m_isHitPlayer;


	//座標
	VECTOR	 m_pos;

	//当たり判定
	Rect m_enemyCollision;

};

