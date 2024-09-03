#pragma once
#include "EnemyBase.h"
#include "Rect.h"
class Boss :public EnemyBase
{
public:
	Boss(int handle,VECTOR playerPos);
	virtual ~Boss();

	void Update(Knight* knight, VECTOR playerPos)override;

	//プレイヤーの位置によって生成する位置を変える
	void SetPosX(VECTOR playerPos)override;

private:

	//攻撃パターン
	void AttackPattern(VECTOR playerPos);

	//近接攻撃
	void Attack(VECTOR playerPos);

	//遠距離攻撃
	void LongRangeAttack(VECTOR playerPos);

	//移動関連
	void Move(Knight* knight);

	//アニメーション関連
	void  Animation();

	//ループしたかどうかを返す
	bool UpdateAnim(int attachNo);

	//アニメーションの変更
	void ChangeAnim(int animIndex);

private:;

	//砲弾のモデル
	int m_cannonballHandle;

	//カウント
	int m_count;

	//アニメーション情報
	int m_currentAnimNo;	//現在のアニメーション
	int m_prevAnimNo;		//変更前のアニメーション
	float m_animBlendRate;	//アニメーション合成割合

	//上空から落ちてくる
	float m_skyAboveY;

	//プレイヤーとの距離
	VECTOR m_distance;

	//砲弾の着弾位置
	VECTOR m_impactPos;

	//近接攻撃の当たり判定
	Rect m_attackCollision;

	//遠距離攻撃の当たり判定
	Rect m_longRangeAttackCollision;
};

