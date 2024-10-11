﻿#include "DxLib.h"
#include "Knight.h"
#include "Bee.h"
#include <math.h>
#include "EffekseerForDXLib.h"

#define PI    3.1415926535897932384626433832795f

namespace
{
	//敵の速さ
	constexpr float kSpeed = 2.0f;

	//敵の最大HP
	constexpr int kMaxHp = 50;

	//ダメージ数
	constexpr int kDamageNum = 10;

	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 4.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//アニメーション
	
	//移動時
	constexpr int kRunAnimIndex = 2;

	

	//発生位置
	constexpr int kAppearanceX = 750;

	//当たり判定の調整
	constexpr float kAdj = 45.0f;

	//当たり判定の大きさ
	constexpr float kModelWidth = 80.0f;
	constexpr float kModelHeight = 100.0f;
	constexpr float kModelDepth = 100.0f;

	//プレイヤーの位置
	constexpr float kPlayerPos1X = -4500.0f;

	constexpr float kPlayerPos2X = 265.0f;

	constexpr float kPlayerPos3X = 6175.0f;

	constexpr float kPlayerPos4X = 12000.0f;

	//ハチの初期位置
	constexpr float kPosY = 240.0f;

	//ノックバック
	constexpr float kKnockback = 50.0f;

	constexpr float kMaxKnockback = 250.0f;

	//無敵時間
	constexpr int  kInvincibleTime = 20; 

	//向いている方向
	constexpr int kRotationStandard1 = 30;

	constexpr int kRotationStandard2 = 120;

	//向いている方向を出す計算
	constexpr float kAngle = 180.0f / PI;

	//HpBarの画像サイズ調整
	constexpr int kHpBarAdjustmentSizeX = 20;
	constexpr int kHpBarAdjustmentSizeY = 150;

}

Bee::Bee(int m_handle,VECTOR playerPos) :EnemyBase(m_handle), m_animBlendRate(-1), m_currentAnimNo(-1), m_prevAnimNo(-1),m_isAttacking(false),m_isAttack(false), m_playerNum(1)
{
	//アニメーションの初期設定
	m_currentAnimNo = MV1AttachAnim(m_handle, kRunAnimIndex, -1, true);

	//HPバーのロードをする
	m_maxHpHandle = LoadGraph("data/UI/HpBar_Enemy.png");

	//HPの初期設定
	m_hp = kMaxHp;

	//無敵時間の設定
	m_invincibleTime = kInvincibleTime;
	
	//初期化
	m_isHitAttack = false;

	m_knockback = 0;

	//座標設定
	SetPosX(playerPos);

	//HPバーのサイズ
	GetGraphSize(m_hpHandle, &m_graphSizeX, &m_graphSizeY);

	m_graphSizeX += kHpBarAdjustmentSizeX;
	m_graphSizeY += kHpBarAdjustmentSizeY;

	//HPの1に対しての幅
	m_hp1Lenght = static_cast<float>(m_graphSizeX) / static_cast<float>(kMaxHp);
}

Bee::~Bee()
{

}
void Bee::Update(Knight* knight,VECTOR playerPos)
{
	if (!m_isHitAttack)
	{
		//プレイヤーに近づく
		ComingPlayer(knight);
		
		//アニメーション
		Animation();
	}

	//HPバーの表示
	DrawHpBar();

	//向いている方向
	DirectionFacing();

	//無敵時間
	InvincibleTime();

	//当たり判定の更新
	m_enemyCollision.SetCenter(m_pos.x - kAdj, m_pos.y, m_pos.z - kAdj, kModelWidth, kModelHeight, kModelDepth);
}


void Bee::HitAttack(Knight* knight)
{
	m_playerAttackCollision = knight->GetAttackCollision();
	m_playerDirection = knight->GetDirection();
	if (m_enemyCollision.IsCollsion(m_playerAttackCollision) && !m_isHitAttack)
	{

		PlaySoundMem(m_damageSE, DX_PLAYTYPE_BACK);

		m_hp -= kDamageNum;

		m_isHitAttack = true;

	}
	else
	{
		m_state = kMove;
	}
}

void Bee::HitPlayer(Knight* knight, Rect playerCollision)
{
	if (m_enemyCollision.IsCollsion(playerCollision))
	{
		knight->HitBody(m_direction);
	}
}

void Bee::SetPosX(VECTOR playerPos)
{
	if (playerPos.x < kPlayerPos1X)
	{
		m_randomPosX = GetRand(kAppearanceX);
		m_randomPosX = m_randomPosX + playerPos.x;
		m_pos = VGet(m_randomPosX, kPosY, 0.0f);
	}
	else if (playerPos.x < kPlayerPos2X)
	{
		m_randomPosX = GetRand(kAppearanceX);
		m_randomPosX = m_randomPosX + playerPos.x;
		m_pos = VGet(m_randomPosX, kPosY, 0.0f);
	}
	else if (playerPos.x < kPlayerPos3X)
	{
		m_randomPosX = GetRand(kAppearanceX);
		m_randomPosX = m_randomPosX + playerPos.x;
		m_pos = VGet(m_randomPosX, kPosY, 0.0f);
	}
	else if (playerPos.x < kPlayerPos4X)
	{
		m_randomPosX = GetRand(kAppearanceX);
		m_randomPosX = m_randomPosX + playerPos.x;
		m_pos = VGet(m_randomPosX, kPosY, 0.0f);
	}
}

void Bee::ComingPlayer(Knight* knight)
{
	VECTOR playerPos = knight->GetPlayerPos();
	//敵の初期位置からターゲット位置に向かうベクトルを生成する
	//始点から終点に向かうベクトルを求める場合、終点の座標-始点の座標で求める
	VECTOR toTarget = VSub(playerPos, m_pos); 

	//ベクトルの長さをkSpeedにしてやる
	//ベクトルの正規化　長さを１にする
	toTarget = VNorm(toTarget);
	//kSpeedでかける
	m_distance.x = toTarget.x * kSpeed;
	m_distance.y = toTarget.y;
	m_distance.z = toTarget.z * kSpeed;

	m_pos = VAdd(m_pos, m_distance);

	if (m_distance.x <= 0.0f  )
	{
		Attack();
	}

	if (m_distance.x > 1.0f)
	{
		m_state = kMove;
	}


	// ３Ｄモデル２から３Ｄモデル１に向かうベクトルを算出
	VECTOR SubVector = VSub(playerPos, m_pos);

	// atan2 を使用して角度を取得
	m_enemyAngle = atan2(SubVector.x, SubVector.z);

	//プレイヤーの方向を向く
	MV1SetRotationXYZ(m_handle, VGet(0.0f, m_enemyAngle + DX_PI_F , 0.0f));


	//ポジションの設定
	MV1SetPosition(m_handle, m_pos);
}

void Bee::Attack()
{
	//m_isAttack = true;
	//m_state = kAttack;
}

void Bee::DirectionFacing()
{
	int rotationDegrees = 0.0f;

	rotationDegrees = m_enemyAngle * kAngle;

	if (rotationDegrees > -kRotationStandard1 && rotationDegrees < kRotationStandard1)
	{
		m_direction = kRight;
	}
	else if (rotationDegrees < -kRotationStandard1 && rotationDegrees > -kRotationStandard2)
	{
		m_direction = kBefore;
	}
	else if (rotationDegrees > kRotationStandard1 && rotationDegrees < kRotationStandard2)
	{
		m_direction = kBehind;
	}

	else if (rotationDegrees > kRotationStandard2 || rotationDegrees < -kRotationStandard1)
	{
		m_direction = kLeft;
	}
}

void Bee::InvincibleTime()
{
	if (m_isHitAttack)
	{
		Knockback(m_playerDirection);
		m_invincibleTime--;
	}

	if (m_invincibleTime <= 0)
	{
		m_invincibleTime = kInvincibleTime;
		m_isHitAttack = false;
	}

}

void Bee::DrawHpBar()
{
	//HPバーの表示
	DrawModiBillboard3D(m_pos, (m_graphSizeX * 0.5f), 150.0f, (m_hp1Lenght * m_decreaseHp) - (m_graphSizeX * 0.5f), 150.0f,
		(m_hp1Lenght * m_decreaseHp) - (m_graphSizeX * 0.5f), static_cast<float>(m_graphSizeY), (m_graphSizeX * 0.5f), static_cast<float>(m_graphSizeY), m_maxHpHandle, true);
}

void Bee::Animation()
{
	if (m_prevAnimNo != -1)
	{
		//test 8フレームで切り替え
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= 1.0f) m_animBlendRate = 1.0f;
		//変更後のアニメーション割合を設定する
		MV1SetAttachAnimBlendRate(m_handle, m_prevAnimNo, 1.0f - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_handle, m_currentAnimNo, m_animBlendRate);
	}
	bool isLoop = UpdateAnim(m_currentAnimNo);
	UpdateAnim(m_prevAnimNo);

	if (m_state == kMove)
	{

		if (isLoop)
		{
			ChangeAnim(kRunAnimIndex);
		}
	}
}

bool Bee::UpdateAnim(int attachNo)
{
	//アニメーションが設定されていないので終了
	if (attachNo == -1) return false;

	//アニメーションを進行させる
	float now = MV1GetAttachAnimTime(m_handle, attachNo);

	//アニメーション進める
	now += 0.5f;

	//現在再生中のアニメーションの総カウントを取得する
	float total = MV1GetAttachAnimTotalTime(m_handle, attachNo);
	bool isLoop = false;

	while (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	//進めた時間に設定
	MV1SetAttachAnimTime(m_handle, attachNo, now);
	return isLoop;
}

void Bee::ChangeAnim(int animIndex)
{
	//さらに古いアニメーションがアタッチされている場合はこの時点で削除しておく
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_handle, m_prevAnimNo);
	}

	//現在再生中の待機アニメーションは変更前のアニメーション扱いに
	m_prevAnimNo = m_currentAnimNo;

	//変更後のアニメーションとして攻撃アニメーションを改めて設定する
	m_currentAnimNo = MV1AttachAnim(m_handle, animIndex, -1, false);

	//切り替えの瞬間は変更前のアニメーションが再生される状態にする
	m_animBlendRate = 0.0f;

	//変更前のアニメーション100%
	MV1SetAttachAnimBlendRate(m_handle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//変更後のアニメーション0%
	MV1SetAttachAnimBlendRate(m_handle, m_currentAnimNo, m_animBlendRate);
}
