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

	//ƒnƒ“ƒhƒ‹
	int m_handle;


	//À•W
	VECTOR m_pos;
};

