#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "EnemyBase.h"

namespace
{
	//モデルのサイズの大きさの調整
	constexpr float kExpansion = 0.75f;


	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//ノックバック
	constexpr int kKnockback = 1;

	constexpr int kMaxKnockback =15;

}

EnemyBase::EnemyBase(int ModelHandle)
{
	//モデルをロードする
	m_handle = ModelHandle;

	//ダメージが入った時のSEのロード
	m_damageSE = LoadSoundMem("data/SE/Cut.mp3");

	//最初の状態は動いている
	m_state = kMove;

	//モデルの大きさを変更
	MV1SetScale(m_handle, VGet(kExpansion, kExpansion, kExpansion));
}

EnemyBase::~EnemyBase()
{

	MV1DeleteModel(m_damageSE);

	DeleteEffekseerEffect(m_bloodHandle);
}

void EnemyBase::Init()
{
	
}

void EnemyBase::Draw()
{
#if _DEBUG
	m_enemyCollision.Draw(0x000000,true);
#endif
	//ポジションの設定
	MV1SetPosition(m_handle, m_pos);

	// ３Ｄモデルの描画
	MV1DrawModel(m_handle);
}

void EnemyBase::Knockback(int direction)
{
	//攻撃を受けてからどれだけ移動するか
	m_knockback += kKnockback;
	//方向によって押される方向が変わる
	if (direction == kRight)
	{
		m_pos.z -= m_knockback;
	}
	if (direction == kBehind)
	{
		m_pos.x -= m_knockback;
	}
	if (direction == kBefore)
	{
		m_pos.x += m_knockback;
	}
	if (direction == kLeft)
	{
		m_pos.z += m_knockback;
	}

	if (m_knockback >= kMaxKnockback)
	{
		m_knockback = 0;
	}

}

void EnemyBase::End()
{
	MV1DeleteModel(m_handle);
}
