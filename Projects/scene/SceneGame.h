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

	//フェードイン、アウト
	void Fade();
private:

	//背景のハンドル
	int m_handle;

	//操作説明
	int m_operationHandle;

	//BGM
	int m_bgm;

	//フェードイン、アウト
	int m_fadeAlpha;

	//カメラのアングル
	float m_cameraAngle;

	//シーン終了フラグ
	bool m_isSceneEnd;

	//ゲームクリアなのかのフラグ
	bool m_isGameClear;

	//プレイヤーに当たったかのフラグ
	bool m_isHitPlayer;

	//プレイヤーのHPが0になったかのフラグ
	bool m_isHp0;

	//ステージをクリアしたか
	bool m_isStageClear;

	//クリアしたか
	bool m_isClear;

	//プレイヤーの座標
	VECTOR m_playerPos;

	//プレイヤーの当たり判定
	Rect m_playerCollision;

	//プレイヤーの攻撃の当たり判定
	Rect m_playerAttackCollision;

};

