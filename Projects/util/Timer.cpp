#include "Timer.h"
#include "DxLib.h"

namespace
{
	//タイマーの初期時間
	constexpr float kTimer = 5400.0f;

	//フォントのサイズ
	constexpr int kFontSize = 32;

	//秒数で見せるため60で割る
	constexpr int kDiv = 60;

	//フォントの初期位置
	constexpr int kPosX = 470;
	constexpr int kPosY = 100;
}

Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::Init()
{
	ChangeFont("しろくまフォント");
	timer = kTimer;
	SetFontSize(kFontSize);

	isTimeUp = false;

}

void Timer::Update()
{
	timer--;

	if (timer <= 0)
	{
		timer = 0.0f;
		isTimeUp = true;
	}

}

void Timer::Draw()
{
	DrawFormatString(kPosX, kPosY,GetColor(255, 255, 255),"残り時間:(%.2f)", timer/ kDiv);

}

void Timer::End()
{
}
