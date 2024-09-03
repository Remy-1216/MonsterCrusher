#pragma once
#include "stagebase.h"
class Stage1 :public StageBase
{
public:
	Stage1(int Handle);

	virtual ~Stage1();

	void Update() override;

private:

	//条件をクリアしないと先に進めない
	void StageSetting(bool clear);

	//先に進めないようにする
	void Hooped();
private:

	int m_handle;
};

