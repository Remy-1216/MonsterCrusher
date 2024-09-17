#include "DxLib.h"
#include "Knight.h"
#include "Pad.h"
#include "EffekseerForDXLib.h"
#include <cmath>

#define PI    3.1415926535897932384626433832795f

namespace 
{
	//当たり判定の大きさ
	//半径
	constexpr float kCapsuleRadius = 400.0f;


	//アニメーション	
	constexpr int kWaitAnimIndex = 1;			//待機中
	constexpr int kRunAnimIndex = 2;			//走る
	constexpr int kAttack1AnimIndex = 30;		//1段階目の攻撃
	constexpr int kAttack2AnimIndex = 32;		//2段階目の攻撃
	constexpr int kAttack3AnimIndex = 33;		//3段階目の攻撃
	constexpr int kAttack4AnimIndex = 31;		//4段階目の攻撃
	constexpr int kAttack5AnimIndex = 37;		//5段階目の攻撃
	constexpr int kAttack6AnimIndex = 36;		//6段階目の攻撃

	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame =4.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//アニメーションの速度
	constexpr float kAnimSpeed = 0.75f;

	//ダメージ量
	constexpr int kDamage = 5;

	//移動量
	constexpr float kSpeed = 20.0f;

	//向いている方向を出す計算
	constexpr float kAngle =  180.0f / PI;

	//プレイヤーの当たり判定の調整
	constexpr float kAdj = 45.0f;	
	
	//攻撃の当たり判定の調整
	constexpr float kAttackLeftXAdj = 50.0f;
	constexpr float kAttackBehindXAdj = 120.0f;
	constexpr float kAttackBeforeXAdj = 45.0f;
	constexpr float kAttackRightXAdj = 55.0f;

	constexpr float kAttackLeftZAdj = 45.0f;
	constexpr float kAttacBehindZAdj = 45.0f;
	constexpr float kAttackBeforeZAdj = 50.0f;
	constexpr float kAttackRightZAdj = 150.0f;

	//当たり判定の大きさ
	constexpr float kModelWidth = 80.0f;
	constexpr float kModelHeight = 100.0f;
	constexpr float kModelDepth = 100.0f;

	//初期座標
	constexpr float kPosY = 120.0f;
	constexpr float kPosX = -8000.0f;

	//最大まですすめるところ
	constexpr float kMaxZ = 1490.0f;

	constexpr float kMaxX = 12000.0f;

	constexpr float kMinX = -8600.0f;

	//モデルのサイズ変更
	constexpr float kExpansion = 100.0f;

	//重力の初期値
	constexpr float kGravity = 0.18f;

	//ジャンプしたときにかかる重力
	constexpr float kGravityPlus = 0.25f;

	//ノックバック
	constexpr float kKnockback = 100.0f;


	//プレイヤーの場所によってエネミーを生成する
	constexpr float kStage1playerPosX = -4500.0f;
	constexpr float kStage2playerPosX = 265.0f;
	constexpr float kStage3playerPosX = 6175.0f;
	constexpr float kStage4playerPosX = 12000.0f;

	//HP
	constexpr int kHpMax =100;

	//HP1に対してどれだけバーを伸ばすか
	constexpr float kDrawSize = 3.5f;

	//無敵時間
	constexpr int  kInvincibleTime = 180;

	//回復量
	constexpr int kRecovery = 20;
}

Knight::Knight() :m_state(kWait), m_animBlendRate(-1), m_currentAnimNo(-1), m_prevAnimNo(-1), m_count(0)
, m_lightPos(VGet(0.0f, 0.0f, 0.0f)),m_killSE(-1),m_damageSE(-1),m_isRun(false),m_pos(VGet(0.0f,0.0f,0.0f))
, m_move(VGet(0.0f, 0.0f, 0.0f)), m_playerAngle(0), m_movementDirection(VGet(0.0f,0.0f,0.0f)),m_knockBack(0)
,m_bloodHandle(-1)
{
	
}

Knight::~Knight()
{
	MV1DeleteModel(m_handle);

	DeleteSoundMem(m_damageSE);

	DeleteSoundMem(m_killSE);
	
	DeleteEffekseerEffect(m_bloodHandle);
}

void Knight::Init()
{
	//モデルのロード
	m_handle = MV1LoadModel("data/model/player/knight.mv1");

	//モデルのサイズ変更
	MV1SetScale(m_handle, VGet(kExpansion, kExpansion, kExpansion));

	//アニメーションの初期設定
	m_currentAnimNo = MV1AttachAnim(m_handle, kWaitAnimIndex, -1, true);

	//初期位置
	m_pos = VGet(kPosX, kPosY, 0.0f);

	//クリアした回数を初期化
	m_stageClear = 0;

	// 再生中のエフェクトのハンドルを初期化する。
	m_playingEffectHandle = -1;

	//プレイヤーが敵に当たった時のエフェクト
	m_bloodHandle = LoadEffekseerEffect("data/effect/blood.efkefc", 1.0f);

	//剣で斬った時の音
	m_killSE = LoadSoundMem("data/SE/kill.mp3");

	//敵と当たった時の音
	m_damageSE = LoadSoundMem("data/SE/damage.mp3");

	//最初は待っている状態
	m_state = kWait;

	//最初は前を向いている状態
	m_direction = kBefore;

	//HPの初期化
	m_hp = kHpMax;

	//ノックバックの値
	m_knockBack = 0;

	//無敵時間の初期化
	m_invincibleTime = kInvincibleTime;


	//フラグたちの初期化
	m_isMove = false;
	m_isAttacking = false;
	m_isAttack = false;
	m_isRun = false;
	m_isHp0 = false;
	m_isHit = false;
}

void Knight::Update(float cameraAngle)
{
	//操作関連
	Operarion(cameraAngle);

	//アニメーション
	Animation();

	//ステージ外に出ないようにする
	StageProcess();

	//ボタンカウントを一定時間で初期化する
	ButtonCountProcess();

	//Hpが0になったかどうか
	HpManager();

	//どっちを向いているか
	DirectionFacing();

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	//モデルの位置更新
	MV1SetPosition(m_handle, m_pos);

	//無敵時間関係
	InvincibleTime();

	//胴体の当たり判定
	m_playerCollision.SetCenter(m_pos.x- kAdj, m_pos.y, m_pos.z- kAdj, kModelWidth, kModelHeight, kModelDepth);
}

void Knight::Draw()
{
	//モデルの表示
	MV1DrawModel(m_handle);

	//HPバーの表示
	DrawHPBar();

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();


#ifdef _DEBUG
	DrawFormatString(160, 400, GetColor(255, 255, 255), "playerの座標(%.2f,%.2f,%.2f)", m_pos.x, m_pos.y, m_pos.z);

	m_playerCollision.Draw(0x000000, true);

	m_attackCollision.Draw(0x000000, true);

	DrawFormatString(0, 100, 0xffffff, "%d", m_isRun);
#endif
}

//ステージ外に出ないようにする
void Knight::StageProcess()
{
	//ステージ外にでないようにする
	if (m_pos.z > kMaxZ)
	{
		m_pos.z = kMaxZ;
	}
	else if (m_pos.z < -kMaxZ)
	{
		m_pos.z = -kMaxZ;
	}

	if (m_pos.y <= kPosY)
	{
		m_pos.y = kPosY;
	}

	if (m_stageClear == 0)
	{
		if (m_pos.x < kMinX)
		{
			m_pos.x = kMinX;
		}
	}
	if (m_stageClear == 1)
	{
		if (m_pos.x < kStage1playerPosX)
		{
			m_pos.x = kStage1playerPosX;
		}
	}
	if (m_stageClear == 2)
	{
		if (m_pos.x < kStage2playerPosX)
		{
			m_pos.x = kStage2playerPosX;
		}
	}
	if (m_stageClear == 3)
	{
		if (m_pos.x < kStage3playerPosX)
		{
			m_pos.x = kStage3playerPosX;
		}
	}
	else if (m_pos.x > kMaxX)
	{
		m_pos.x = kMaxX;
	}
}

//どっち向いているのか
void Knight::DirectionFacing()
{
	int rotationDegrees = 0.0f;

	rotationDegrees = m_playerAngle * kAngle;

	if (rotationDegrees > -30 && rotationDegrees < 30)
	{
		m_direction = kRight;
	}
	else if (rotationDegrees < 130 && rotationDegrees > 70)
	{
		m_direction = kBehind;
	}
	else if (rotationDegrees < -60 && rotationDegrees > -120)
	{
		m_direction  = kBefore;
	}
	else if (rotationDegrees > 135 || rotationDegrees < -130)
	{
		m_direction = kLeft;
	}
}

//ボタンを押した回数を一定時間で初期化する
void Knight::ButtonCountProcess()
{
	//ボタンのカウントを一定時間で初期化する
	m_count++;
	if (m_count >= 180)
	{
		m_attackCollision.SetCenter(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		m_countXButton = 0;
		m_count = 0;
	}
}

//ボタンの割り当て
void Knight::Operarion(float cameraAngle)
{
	m_move = VGet(0.0f, 0.0f, 0.0f);

	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		m_state = kMove;
		m_move.x += kSpeed;

		m_attackCollision.SetCenter(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}
	if (Pad::IsRelase(PAD_INPUT_RIGHT))
	{
		m_state = kWait;
	}

	//移動
	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		m_state = kMove;
		m_move.x -= kSpeed;

		m_attackCollision.SetCenter(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}
	if (Pad::IsRelase(PAD_INPUT_LEFT))
	{
		m_state = kWait;
	}

	//移動
	if (Pad::IsPress(PAD_INPUT_UP))
	{
		m_state = kMove;
		m_move.z += kSpeed;

		m_attackCollision.SetCenter(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}
	if (Pad::IsRelase(PAD_INPUT_UP))
	{		
		m_state = kWait;
	}

	//移動
	if (Pad::IsPress(PAD_INPUT_DOWN))
	{
		m_state = kMove;
		m_move.z -= kSpeed;

		m_attackCollision.SetCenter(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}
	if (Pad::IsRelase(PAD_INPUT_DOWN))
	{
		m_state = kWait;	
	}
	
	//コントローラーのXボタン
	if (Pad::IsTrigger(PAD_INPUT_C))
	{
		m_state = kAttack;
		m_isAttack = true;
		Attack();
		m_countXButton++;
	}
	if (Pad::IsRelase(PAD_INPUT_C))
	{
		m_state = kWait;
	}

	// 移動ベクトルがゼロでない場合、プレイヤーの向きを更新
	if (m_move.x != 0.0f || m_move.z != 0.0f) 
	{
		m_move = VNorm(m_move);

		// カメラの回転を基準に方向を変換
		m_rotMatrix = MGetRotY(cameraAngle);
		m_movementDirection = VTransform(m_move, m_rotMatrix);
		
		//プレイヤーの移動処理
		m_pos = VAdd(m_pos, VScale(m_movementDirection, kSpeed));

		// プレイヤーの向きを設定
		m_playerAngle = atan2f(-m_movementDirection.x, -m_movementDirection.z);
	}

	//モデルの回転更新
	MV1SetRotationXYZ(m_handle, VGet(0.0f, m_playerAngle, 0.0f));
}

//アニメーション
void Knight::Animation()
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


	if (m_state == kWait)
	{
		m_isRun = false;
		m_isAttack = false;
		m_isMove = false;
		m_isAttacking = false;

		//待機中なら待機モーションを行う
		if (isLoop)
		{
			ChangeAnim(kWaitAnimIndex);
		}
	}
	if (m_state == kMove)
	{
		m_isRun = true;
		//動いているなら走るモーションを行う
		if (m_isMove != m_isRun)
		{
			m_isMove = m_isRun;
			if (m_isMove)
			{
				ChangeAnim(kRunAnimIndex);
			}
		}
	}
	if (m_state == kAttack)
	{
		if (m_isAttacking != m_isAttack)
		{
			m_isAttacking = m_isAttack;
			if (m_isAttacking && m_countXButton == 1)
			{
				ChangeAnim(kAttack1AnimIndex);
			}
			if (m_isAttacking && m_countXButton == 2)
			{
				ChangeAnim(kAttack2AnimIndex);
			}
			if (m_isAttacking && m_countXButton == 3)
			{
				ChangeAnim(kAttack3AnimIndex);
			}
			if (m_isAttacking && m_countXButton == 4)
			{
				ChangeAnim(kAttack4AnimIndex);
			}
			if (m_isAttacking && m_countXButton == 5)
			{
				ChangeAnim(kAttack5AnimIndex);
			}
			if (m_isAttacking && m_countXButton == 6)
			{
				ChangeAnim(kAttack6AnimIndex);
				m_countXButton = 0;
			}
		}
	}
}

bool Knight::UpdateAnim(int attachNo)
{
	//アニメーションが設定されていないので終了
	if (attachNo == -1) return false;

	//アニメーションを進行させる
	float now = MV1GetAttachAnimTime(m_handle, attachNo);

	//アニメーション進める
	now += kAnimSpeed;

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

void Knight::ChangeAnim(int animIndex)
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

//攻撃
void Knight::Attack()
{
	PlaySoundMem(m_killSE, DX_PLAYTYPE_BACK, true);

	if (m_direction == kBefore)
	{
		m_attackCollision.SetCenter(m_pos.x + kAttackBeforeXAdj, m_pos.y, m_pos.z - kAttackBeforeZAdj, kModelWidth, kModelHeight, kModelDepth);
	}
	if (m_direction == kBehind)
	{
		m_attackCollision.SetCenter(m_pos.x - kAttackBehindXAdj, m_pos.y, m_pos.z - kAttacBehindZAdj, kModelWidth, kModelHeight, kModelDepth);
	}
	if (m_direction == kRight)
	{
		m_attackCollision.SetCenter(m_pos.x - kAttackRightXAdj, m_pos.y, m_pos.z - kAttackRightZAdj, kModelWidth, kModelHeight, kModelDepth);
	}
	if (m_direction == kLeft)
	{
		m_attackCollision.SetCenter(m_pos.x - kAttackLeftXAdj, m_pos.y, m_pos.z + kAttackLeftZAdj, kModelWidth, kModelHeight, kModelDepth);
	}

}

//HPバーの描画
void Knight::DrawHPBar()
{
	// HP の値を文字列とバーで表示
	DrawFormatString(50, 50, GetColor(255, 255, 255), "%4d", m_hp);
	// HP の値分の大きさだが四角に収まるように値を大きくします
	DrawFillBox(200, 50, 200 + m_hp	 * kDrawSize, 66, GetColor(77, 181, 106));
	DrawLineBox(200, 50, 450 + kHpMax, 66, GetColor(77, 181, 0));
}

//無敵時間
void Knight::InvincibleTime()
{
	if(m_isHit)
	{
		m_invincibleTime--;
	}


	if (m_invincibleTime <= 0)
	{
		m_isHit =false;
		m_invincibleTime = kInvincibleTime;
	}
}

void Knight::LeaningBack()
{
	if (m_direction == kRight)
	{
		m_pos.z += m_knockBack;
	}
	if (m_direction == kBehind)
	{
		m_pos.x += m_knockBack;
	}
	if (m_direction == kBefore)
	{
		m_pos.x -= m_knockBack;
	}
	if (m_direction == kLeft)
	{
		m_pos.z -= m_knockBack;
	}

}

//敵と当たった時の処理
void Knight::HitBody()
{	
	if (!m_isHit)
	{
		m_isHit = true;
		
		m_hp = m_hp-kDamage;

		PlaySoundMem(m_damageSE, DX_PLAYTYPE_BACK);

		m_knockBack++;

		// エフェクトを再生する。
		m_playingEffectHandle = PlayEffekseer3DEffect(m_bloodHandle);

		
		// 再生中のエフェクトを移動する。
		SetPosPlayingEffekseer3DEffect(m_playingEffectHandle, m_pos.x, m_pos.y + 100, m_pos.z);

		if (m_knockBack <= kKnockback)
		{
			m_knockBack = 0;
		}

	}
}

//HPが0になった時の処理
void Knight::HpManager()
{
	if (m_hp <= 0)
	{
		m_isHp0 -= true;
	}
}

//クリアしたらステージ外に出る位置を帰る
void Knight::ConditionCleared(bool clear)
{
	if (clear)
	{
		m_stageClear++;

		m_hp += kRecovery;

		if (m_hp >= kHpMax)
		{
			m_hp = kHpMax;
		}
	}
	else
	{
		if (m_stageClear == 0)
		{
			if (m_pos.x > kStage1playerPosX)
			{
				m_pos.x = kStage1playerPosX;
			}
		}
		if (m_stageClear == 1)
		{
			if (m_pos.x > kStage2playerPosX)
			{
				m_pos.x = kStage2playerPosX;
			}
		}
		if (m_stageClear == 2)
		{
			if (m_pos.x > kStage3playerPosX)
			{
				m_pos.x = kStage3playerPosX;
			}
		}
		if (m_stageClear == 3)
		{
			if (m_pos.x > kStage4playerPosX)
			{
				m_pos.x = kStage4playerPosX;
			}
		}
	}
}

