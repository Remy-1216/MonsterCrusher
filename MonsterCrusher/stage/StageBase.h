#pragma once
class StageBase
{
public:
	StageBase(int ModelHandle);
	virtual ~StageBase();

	void Init();

	virtual void Update()abstract;

	void Draw();

protected:

	//ハンドル
	int m_handle;


	//座標
	VECTOR m_pos;
};

