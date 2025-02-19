#pragma once

//パッドの状態を取得する
namespace Pad
{
	//パッドの更新処理　1フレームに１回行う
	void Update();

	//押されているか処理
	bool IsPress(int key);
	//押された瞬間を取得
	bool IsTrigger(int key);
	//離した瞬間の処理
	bool IsRelase(int key);
}

