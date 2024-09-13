#pragma once
#include "enemybase.h"
#include "Rect.h"
class Slime :public EnemyBase
{
public:
	Slime(int m_handle, VECTOR playerPos);
	virtual ~Slime();

	void Update(Knight* knigh, VECTOR playerPos)override;

	//プレイヤーの位置によって生成する位置を変える
	void SetPosX(VECTOR playerPos)override;

	//攻撃が当たった時の処理
	void HitAttack(Rect playerAttack);

	//プレイヤーにあたる
	void HitPlayer(Knight*knight, Rect playerCollision);

	//スライムの当たり判定を与える
	Rect GetSlimeCollision() const noexcept { return m_enemyCollision; }

	//スライムの体力を与える
	int GetSlimeHp() const noexcept { return m_hp; }

	//当たったかどうかを与える
	bool GetHitPlayer() const noexcept { return m_isHitPlayer; }

private:
	//プレイヤーに少しづつ近づく
	void ComingPlayer(Knight* knight);

	//無敵
	void InvincibleTime();

	//アニメーション関連
	void  Animation();

	//ループしたかどうかを返す
	bool UpdateAnim(int attachNo);

	//アニメーションの変更
	void ChangeAnim(int animIndex);

	

	
private:
	//アニメーション情報
	int m_currentAnimNo;	//現在のアニメーション
	int m_prevAnimNo;		//変更前のアニメーション
	float m_animBlendRate;	//アニメーション合成割合

	//ランダムな位置
	float m_randomPosX;

	//進む距離
	VECTOR m_distance;
};

