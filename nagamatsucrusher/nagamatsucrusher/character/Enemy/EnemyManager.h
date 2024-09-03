#pragma once
#include <vector>
#include <memory>
#include "Rect.h"
class Knight;
class Bee;
class Slime;
class EnemyManager
{
public:
	EnemyManager();
	virtual ~EnemyManager();

	void Init();

	void Update(Knight* knight, VECTOR playerPos, Rect playerCollision,Rect playerAttackCollision);

	//描画
	void Draw();	

	//クリアしたかを渡す
	bool GetStageClear() const noexcept { return m_isStageClear; }
private:

	//敵の生成
	void CreateEnemy(VECTOR playerPos);

	//ハチの生成
	void CreateBee(VECTOR playerPos);

	//スライムの生成
	void CreateSlime(VECTOR playerPos);

	//残りの人数を表示
	void DrawEnemys();

	//クリアしたかどうか
	void StageClear(VECTOR playerPos);

private:
	static constexpr int kEnemyNum = 10;

private:

	Bee * m_pBee[kEnemyNum];

	Slime* m_pSlime[kEnemyNum];

	//判定
	//ハチの当たり判定
	Rect m_beeCollision[kEnemyNum];

	//スライムの当たり判定
	Rect m_slimeCollision[kEnemyNum];

	//HPを保存
	//ハチのHP
	int m_beeHp[kEnemyNum];

	//スライムのHP
	int m_slimeHp[kEnemyNum];

private:
	//蜂のモデル
	int m_beeHandle;

	//スライムのモデル
	int m_slimeHandle;

	//どちらの敵を生成するか
	int m_randomEnemy;

	//現在の敵の数
	int m_enemyNum;

	//ステージ1の敵の数
	int m_enemyNumRetention1;

	//ステージ2の敵の数
	int m_enemyNumRetention2;

	//ステージ3の敵の数
	int m_enemyNumRetention3;

	//ステージ4の敵の数
	int m_enemyNumRetention4;

	//タイマー
	int m_timer;

	//ステージをClearしたか
	bool m_isStageClear;

	//ステージ1をクリアした
	bool m_isClear1;

	//ステージ2をクリアした
	bool m_isClear2;

	//ステージ3をクリアした
	bool m_isClear3;

	//ステージ4をクリアした
	bool m_isClear4;

	//プレイヤーの座標保持
	VECTOR m_playerPos;

};

