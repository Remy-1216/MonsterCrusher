#pragma once
#include "EnemyBase.h"
#include "Rect.h"
class Knight;
class Bee : public EnemyBase
{
public:
	Bee(int m_handle, VECTOR playerPos);
	virtual ~Bee();

	void Update(Knight* knight,VECTOR playerPos) override;

	//プレイヤーの位置によって生成する位置を変える
	void SetPosX(VECTOR playerPos)override;

	//攻撃が当たった時の処理
	void HitAttack(Rect playerAttack);

	//プレイヤーにあたる
	void HitPlayer(Knight* knight, Rect playerCollision);
	
	//ハチの体力を与える
	int GetBeeHp() const noexcept { return m_hp; }

	//ハチの当たり判定を与える
	Rect GetBeeCollision() const noexcept { return m_enemyCollision; }

	//当たったかどうかを与える
	bool GetHitPlayer() const noexcept { return m_isHitPlayer; }


private:
	//プレイヤーに少しづつ近づく
	void ComingPlayer(Knight* knight);

	//プレイヤーに近づいたら攻撃
	void Attack();

	//アニメーション関連
	void  Animation();

	//ループしたかどうかを返す
	bool UpdateAnim(int attachNo);

	//アニメーションの変更
	void ChangeAnim(int animIndex);


private:
	MV1_COLL_RESULT_POLY* m_pPlayerNum[1];	// プレイヤーと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
private:
	//アニメーション情報
	int m_currentAnimNo;	//現在のアニメーション
	int m_prevAnimNo;		//変更前のアニメーション
	float m_animBlendRate;	//アニメーション合成割合

	//プレイヤーの人数
	int m_playerNum;

	//ランダムな位置
	float m_randomPosX;

	//攻撃しているかを保持する
	bool m_isAttacking;

	//攻撃しているかどうか
	bool m_isAttack;

	//描画される蜂の座標
	VECTOR m_beePos;

	//進む距離
	VECTOR m_distance;

};

