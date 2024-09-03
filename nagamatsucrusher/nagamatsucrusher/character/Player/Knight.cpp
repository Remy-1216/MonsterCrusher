#include "DxLib.h"
#include "Knight.h"
#include "Pad.h"
#include <cmath>

namespace 
{
	//アニメーション	
	constexpr int kWaitAnimIndex = 1;			//待機中
	constexpr int kRunAnimIndex = 2;			//走る
	constexpr int kAttack1AnimIndex = 30;		//1段階目の攻撃
	constexpr int kAttack2AnimIndex = 32;		//2段階目の攻撃
	constexpr int kAttack3AnimIndex = 33;		//3段階目の攻撃
	constexpr int kAttack4AnimIndex = 31;		//4段階目の攻撃
	constexpr int kAttack5AnimIndex = 37;		//5段階目の攻撃
	constexpr int kAttack6AnimIndex = 36;		//6段階目の攻撃
	constexpr int kAerialSlashAnimIndex = 39;	//空中斬り
	constexpr int kAttackMagicAnimIndex = 61;	//魔法攻撃

	//ジャンプ力
	constexpr float kJumpPower = 10.0f;

	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame =4.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//移動量
	constexpr float kSpped = 15.0f;

	//回転量
	constexpr float kRotateRight = 270.0f * DX_PI_F / 180.0f;
	constexpr float kRotateLeft = 90.0f * DX_PI_F / 180.0f;
	constexpr float kRotateUp = 180.0f * DX_PI_F / 180.0f;
	constexpr float kRotateDown = 0.0f;

	//プレイヤーの当たり判定の調整
	constexpr float kAdj = 45.0f;	
	
	//攻撃の当たり判定の調整
	constexpr float kAttackLeftXAdj = 100.0f;
	constexpr float kAttackRightXAdj = 150.0f;
	constexpr float kAttackUpXAdj = 45.0f;
	constexpr float kAttackDownXAdj = 45.0f;

	constexpr float kAttackLeftZAdj = 45.0f;
	constexpr float kAttackRightZAdj = 45.0f;
	constexpr float kAttackUpZAdj = 50.0f;
	constexpr float kAttackDownZAdj = 150.0f;

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
	constexpr float kKnockback = 250.0f;


	//プレイヤーの場所によってエネミーを生成する
	constexpr float kStage1playerPosX = -4500.0f;

	constexpr float kStage2playerPosX = 265.0f;

	constexpr float kStage3playerPosX = 6175.0f;

	constexpr float kStage4playerPosX = 12000.0f;

	//HP
	constexpr int kHpMax = 100;

	//HP1に対してどれだけバーを伸ばすか
	constexpr float kDrawSize = 3.5f;
}

Knight::Knight() :m_state(kWait), m_animBlendRate(-1), m_currentAnimNo(-1), m_prevAnimNo(-1), m_count(0)
, m_lightPos(VGet(0.0f, 0.0f, 0.0f)),m_killSE(-1),m_damageSE(-1),m_isRun(false),m_pos(VGet(0.0f,0.0f,0.0f))
{
	m_handle = MV1LoadModel("data/model/player/knight.mv1");
}

Knight::~Knight()
{
	MV1DeleteModel(m_handle);

	DeleteSoundMem(m_damageSE);

	DeleteSoundMem(m_killSE);
}

void Knight::Init()
{
	MV1SetScale(m_handle, VGet(kExpansion, kExpansion, kExpansion));

	//アニメーションの初期設定
	m_currentAnimNo = MV1AttachAnim(m_handle, kWaitAnimIndex, -1, true);

	m_pos = VGet(kPosX, kPosY, 0.0f);

	m_stageClear = 0;

	m_killSE = LoadSoundMem("data/SE/kill.mp3");

	m_damageSE = LoadSoundMem("data/SE/damage.mp3");

	m_state = kWait;

	m_direction = kLeft;

	m_hp = kHpMax;

	m_isMove = false;
	m_isAttacking = false;
	m_isAttack = false;
	m_isRun = false;
	m_isHp0 = false;
}

void Knight::Update(VECTOR cameraPos)
{
	//操作関連
	Operarion(cameraPos);

	//アニメーション関連
	Animation();

	//ジャンプした後の処理
	JumpProcess();

	//ステージ外に出ないようにする
	StageProcess();

	//ボタンカウントを一定時間で初期化する
	ButtonCountProcess();

	//Hpが0になったかどうか
	HpManager();

	//モデルの位置更新
	MV1SetPosition(m_handle, m_pos);


	m_playerCollision.SetCenter(m_pos.x- kAdj, m_pos.y, m_pos.z- kAdj, kModelWidth, kModelHeight, kModelDepth);
}

void Knight::Draw()
{
	MV1DrawModel(m_handle);

	DrawHPBar();

#ifdef _DEBUG
	DrawFormatString(160, 400, GetColor(255, 255, 255), "playerの座標(%.2f,%.2f,%.2f)", m_pos.x, m_pos.y, m_pos.z);

	m_playerCollision.Draw(0x000000, true);

	m_attackCollision.Draw(0x000000, true);
#endif
}

//ジャンプした時の処理
void Knight::JumpProcess()
{
	//ジャンプした後の処理
	if (m_pos.y > kPosY)
	{
		m_pos = VSub(m_pos, VGet(0, m_gravity, 0));
		m_gravity += kGravityPlus;
	}
	else
	{
		m_gravity = kGravity;
		m_state = kWait;
	}
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

//ボタンを押した回数を一定時間で初期化する
void Knight::ButtonCountProcess()
{
	//ボタンのカウントを一定時間で初期化する
	m_count++;

	if (m_count >= 60)
	{
		m_countAButton = 0;

		
	}
	if (m_count >= 180)
	{
		m_attackCollision.SetCenter(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		m_countXButton = 0;
		m_count = 0;
	}
}

//ボタンの割り当て
void Knight::Operarion(VECTOR cameraPos)
{
	//m_cameraPos.x = cameraPos.x -m_pos.x;

	//m_cameraPos.y = cameraPos.y -m_pos.y;

	//m_cameraPos.z = cameraPos.z -m_pos.z;

	//m_cameraPos.x = m_cameraPos.x * m_cameraPos.x;
	//
	//m_cameraPos.y = m_cameraPos.y * m_cameraPos.y;
	//
	//m_cameraPos.z = m_cameraPos.z * m_cameraPos.z;

	//m_cameraPos = VSub(m_pos, m_cameraPos);

	//m_cameraPos = VNorm(m_cameraPos);

	//m_cameraPos = VScale(m_cameraPos, 180);


	//移動
	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		m_state = kMove;
		m_direction = kDown;
		m_pos = VAdd(m_pos, VGet(0.0f,
			0.0f, 
			-kSpped));

		MV1SetRotationXYZ(m_handle, VGet(0, kRotateDown, 0));

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
		m_direction = kUp;
		m_pos = VAdd(m_pos, VGet(0.0f,
			0.0f,
			kSpped));

		MV1SetRotationXYZ(m_handle, VGet(0,kRotateUp,0));

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
		m_direction = kRight;
		m_pos = VAdd(m_pos, VGet(kSpped, 
			0.0f,
			0.0f));

		MV1SetRotationXYZ(m_handle, VGet(0, kRotateRight,0));

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
		m_direction = kLeft;
		m_pos = VAdd(m_pos, VGet(-kSpped,
			0.0f,
			0.0f));

		MV1SetRotationXYZ(m_handle, VGet(0,kRotateLeft, 0));

		m_attackCollision.SetCenter(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}
	if (Pad::IsRelase(PAD_INPUT_DOWN))
	{
		m_state = kWait;	
	}

	//コントローラーのXボタン
	if (Pad::IsTrigger(PAD_INPUT_C))
	{
		m_isAttack = true;
		Attack();
		m_countXButton++;
		m_state = kAttack;
	}
	if (Pad::IsRelase(PAD_INPUT_C))
	{
		m_state = kWait;
	}

	//コントローラーのAボタン
	if (Pad::IsTrigger(PAD_INPUT_A))
	{
		m_countAButton++;
		m_state = kAerialSlash;
	}

	if (Pad::IsTrigger(PAD_INPUT_B))
	{
		m_state = kAttackMagic;
	}
	
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
		if (m_isAttacking !=m_isAttack)
		{
			m_isAttacking = m_isAttack;
			if(m_isAttacking && m_countXButton == 1)
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
	if (m_state == kAerialSlash)
	{
		m_pos = VAdd(m_pos, VGet(0, kJumpPower, 0));

		if (m_countAButton == 2)
		{
			ChangeAnim(kAerialSlashAnimIndex);
			m_countAButton = 0;
		}
	}
	if (m_state == kAttackMagic)
	{
		ChangeAnim(kAttackMagicAnimIndex);
	}
}

bool Knight::UpdateAnim(int attachNo)
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

	if (m_direction == kRight)
	{
		m_attackCollision.SetCenter(m_pos.x + kAttackLeftXAdj, m_pos.y, m_pos.z-kAttackLeftZAdj , kModelWidth, kModelHeight, kModelDepth);
	}
	if (m_direction == kLeft)
	{
		m_attackCollision.SetCenter(m_pos.x - kAttackRightXAdj, m_pos.y, m_pos.z-kAttackRightZAdj , kModelWidth, kModelHeight, kModelDepth);
	}
	if (m_direction == kDown)
	{
		m_attackCollision.SetCenter(m_pos.x-kAttackDownXAdj, m_pos.y, m_pos.z - kAttackDownZAdj, kModelWidth, kModelHeight, kModelDepth);
	}
	if (m_direction == kUp)
	{
		m_attackCollision.SetCenter(m_pos.x - kAttackUpXAdj, m_pos.y, m_pos.z + kAttackUpZAdj, kModelWidth, kModelHeight, kModelDepth);
	}
}

//HPバーの描画
void Knight::DrawHPBar()
{

	// HP の値を文字列とバーで表示
	DrawFormatString(50, 50, GetColor(255, 255, 0), "%4d", m_hp);
	// HP の値分の大きさだが四角に収まるように値を大きくします
	DrawFillBox(200, 50, 200 + m_hp	 * kDrawSize, 66, GetColor(77, 181, 106));
	DrawLineBox(200, 50, 450 + kHpMax, 66, GetColor(77, 181, 0));
}

//void Knight::AttackMagic()
//{
//}

//敵と当たった時の処理
void Knight::HitBody()
{	
	m_hp -= 5;

	PlaySoundMem(m_damageSE, DX_PLAYTYPE_BACK);

	if (m_direction == kUp)
	{
		m_pos.z += 250.0f;
	}
	if(m_direction == kDown)
	{
		m_pos.z -= 250.0f;
	}
	if (m_direction == kRight)
	{
		m_pos.x += 250.0f;
	}
	if (m_direction == kLeft)
	{
		m_pos.x -= 250.0f;
	}
	

}

//HPが0になった時の処理
void Knight::HpManager()
{
	if (m_hp <= 0)
	{
		//m_isHp0 -= true;
	}
}

void Knight::ConditionCleared(bool clear)
{
	if (clear)
	{
		m_stageClear++;
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

