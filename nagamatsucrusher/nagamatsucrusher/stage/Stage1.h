#pragma once
#include "stagebase.h"
class Stage1 :public StageBase
{
public:
	Stage1(int Handle);

	virtual ~Stage1();

	void Update() override;

private:

	//�������N���A���Ȃ��Ɛ�ɐi�߂Ȃ�
	void StageSetting(bool clear);

	//��ɐi�߂Ȃ��悤�ɂ���
	void Hooped();
private:

	int m_handle;
};

