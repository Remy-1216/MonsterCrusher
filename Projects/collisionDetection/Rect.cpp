#include "Rect.h"
#include "DxLib.h"
#include <cassert>

Rect::Rect() :
	m_left(0.0f),
	m_top(0.0f),
	m_right(0.0f),
	m_bottom(0.0f)
{
}

Rect::~Rect()
{
}

void Rect::Draw(unsigned int Color, bool isFill)
{
	DrawBox(static_cast<int>(m_left), static_cast<int>(m_top), static_cast<int>(m_right), static_cast<int>(m_bottom), Color, isFill);
}

void Rect::SetCenter(float x, float y, float z,float width, float height)
{
	//ワールド座標からスクリーン座標に変換
	VECTOR screenPos = ConvWorldPosToScreenPos(VGet(x, y,z));

	m_left = screenPos.x - width *0.5f;
	m_top = screenPos.y - height * 0.5f;
	m_right = screenPos.x + width * 0.5f;
	m_bottom = screenPos.y + height * 0.5f;
}

float Rect::GetWindth() const
{
	assert(m_right >= m_left);	//左右の座標入れ替わりチェック

	return m_right - m_left;
}

float Rect::GetHeight() const
{
	assert(m_bottom >= m_top);	//上下の座標の入れ替わりチェック

	return m_bottom - m_top;
}

VECTOR Rect::GetCenter() const
{
	//　中心座標
	float x = (m_left + m_right) * 0.5f;
	float y = (m_top + m_bottom) *0.5f;
	float z = 0.0f;

	return VECTOR{ x, y, z};
}

bool Rect::IsCollsion(const Rect& rect)
{
	//絶対に当たらないパターンをはじいていく
	if (m_left > rect.m_right) return false;
	if (m_top > rect.m_bottom) return false;
	if (m_right < rect.m_left) return false;
	if (m_bottom < rect.m_top)  return false;
	//当たらないパターン以外は当たっている

	return true;
}