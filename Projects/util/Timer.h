#pragma once
class Timer
{
public:
	Timer();
	~Timer();

	void Init();
	void Update();
	void Draw();
	void End();

	float GetTimer() const { return timer; }

private:

	//フォントのハンドル
	int FontHandle;

	//30秒立ったか
	bool isTimeUp;

	//タイマー
	float timer;
};

