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

	//�n���h��
	int m_handle;


	//���W
	VECTOR m_pos;
};

