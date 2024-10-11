#include "DxLib.h"
#include "StageBase.h"

namespace
{
	//右を向かせる
	constexpr float kRotateH = 90.0f;

	//ラジアンに変更するための計算に必要な数字
	constexpr float kDegrees = 180.0f;
}

StageBase::StageBase(int ModelHandle)
{
	m_handle = ModelHandle;
}

StageBase::~StageBase()
{
	MV1DeleteModel(m_handle);
}

void StageBase::Init()
{
	//モデルを回転させて、右を向かせる
	MV1SetRotationXYZ(m_handle, VGet(0.0f, kRotateH * DX_PI_F / kDegrees, 0.0f));
}

void StageBase::Draw()
{
	//ポジションの設定
	MV1SetPosition(m_handle, m_pos);

	// ３Ｄモデルの描画
	MV1DrawModel(m_handle);
}
