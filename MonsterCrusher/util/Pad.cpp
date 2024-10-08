#include "Pad.h"
#include "DxLib.h"
namespace
{
	//前のフレームのパッド押し下げ状態
	int lastPad = 0;
	//このフレームのパッド押し下げ状態
	int nowPad = 0;
}


namespace Pad
{
	void Update()
	{
		//前のフレームに取得したパッド情報を一つ古い情報にする
		lastPad = nowPad;
		//現在のパッドの情報を取得する
		nowPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	}
	bool IsPress(int key)
	{
		return (nowPad & key);
	}
	bool IsTrigger(int key)
	{
		bool isNow = (nowPad & key);  //このフレーム
		bool isLast = (lastPad & key); //前のフレーム

		if (isNow && !isLast)			//このフレームに押されていて前回のフレームに押されていない
		{
			return true;
		}
		return false;
	}
	bool IsRelase(int key)
	{
		bool isNow = (nowPad & key);  //このフレーム
		bool isLast = (lastPad & key); //前のフレーム

		if (!isNow && isLast)
		{
			return true;
		}
		return false;
	}
}
