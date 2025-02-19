#pragma once
#include "DxLib.h"
// 矩形の座標を管理するためのクラス
class Rect
{
public:
	Rect();
	virtual ~Rect();

	//四角の描画
	void Draw(unsigned int Color, bool isFill);

	//中心座標と幅高さを指定
	void SetCenter(float x, float y, float z,float width, float height);

	//必要な情報を取得する
	float GetWindth()const;		//矩形の幅
	float GetHeight() const;		//矩形の高さ
	VECTOR GetCenter() const;	//矩形の中心座標

	//矩形の同士の当たり判定
	bool IsCollsion(const Rect& rect);

private:
	float m_left;  //左上のX座標
	float m_top;   //左上のY座標
	float m_right; //右下のX座標
	float m_bottom;//右下のY座標
};
