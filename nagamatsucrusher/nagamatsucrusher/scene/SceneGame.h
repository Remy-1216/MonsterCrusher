#pragma once
#include <vector>
#include "SceneBase.h"
#include "DxLib.h"
#include "Rect.h"
class Knight;
class Camera;
class EnemyManager;
class StageManager;
class SceneGame :public SceneBase
{
public:
	SceneGame();
	~SceneGame();

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();
private:

	Knight* m_pPlayer;
	Camera* m_pCamera;
	EnemyManager* m_pEnemyManager;
	StageManager* m_pStageManager;
private:
	//最後の敵を倒せたか
	void GameClear();
private:
	//背景のハンドル
	int m_handle;

	//BGM
	int m_bgm;

	//ゲームクリアなのかのフラグ
	bool m_isGameClear;

	//プレイヤーに当たったかのフラグ
	bool m_isHitPlayer;

	//プレイヤーのHPが0になったかのフラグ
	bool m_isHp0;

	//ステージをクリアしたか
	bool m_isStageClear;

	//プレイヤーの座標
	VECTOR m_playerPos;

	//カメラの座標
	VECTOR m_cameraPos;

	//プレイヤーの当たり判定
	Rect m_playerCollision;

	//プレイヤーの攻撃の当たり判定
	Rect m_playerAttackCollision;

};

