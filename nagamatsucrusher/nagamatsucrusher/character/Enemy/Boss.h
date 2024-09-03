#pragma once
#include "EnemyBase.h"
class Boss :public EnemyBase
{
public:
	Boss(int handle,VECTOR playerPos);
	virtual ~Boss();

	void Update(Knight* knight, VECTOR playerPos)override;

	//プレイヤーの位置によって生成する位置を変える
	void SetPosX(VECTOR playerPos)override;

private:
	//攻撃
	void Attack(Knight* knight);

	//移動関連
	void Move(Knight* knight);

	//アニメーション関連
	void  Animation();

	//ループしたかどうかを返す
	bool UpdateAnim(int attachNo);

	//アニメーションの変更
	void ChangeAnim(int animIndex);

private:
	//HP
	int m_hp;

	//砲弾のモデル
	int m_cannonballHandle;

	//アニメーション情報
	int m_currentAnimNo;	//現在のアニメーション
	int m_prevAnimNo;		//変更前のアニメーション
	float m_animBlendRate;	//アニメーション合成割合

	//ランダムな位置
	float m_randomPosX;

	//砲弾の着弾位置
	VECTOR m_impactPos;
};

