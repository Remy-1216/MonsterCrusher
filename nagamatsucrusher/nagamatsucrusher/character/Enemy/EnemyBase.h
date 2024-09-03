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

	//ダメージ
	void OnDamage();

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

	//プレイヤーの攻撃ヒット回数
	int m_attackHits;

	//プレイヤーの人数
	int m_playerNum;

	//攻撃に当たったかどうか
	bool m_isHitAttack;

	//プレイヤーに当たったかどうか
	bool m_isHitPlayer;


	//座標
	VECTOR	 m_pos;

	//当たり判定
	Rect m_enemyCollision;

};

