#pragma once
#include "StageBase.h"
class Stage1 :public StageBase
{
public:
	Stage1(int Handle);

	virtual ~Stage1();

	void Update() override;

private:

	//æ‚Éi‚ß‚È‚¢‚æ‚¤‚É‚·‚é
	void Hooped();
private:

	int m_handle;
};

