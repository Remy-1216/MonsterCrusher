#pragma once
#include "StageBase.h"
class Stage1 :public StageBase
{
public:
	Stage1(int Handle);

	virtual ~Stage1();

	void Update() override;

private:

	//��ɐi�߂Ȃ��悤�ɂ���
	void Hooped();
private:

	int m_handle;
};

