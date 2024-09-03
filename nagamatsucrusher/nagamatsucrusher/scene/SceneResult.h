#pragma once
#include "SceneBase.h"
class SceneResult :public SceneBase
{
public:
	SceneResult();
	~SceneResult();

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

private:
	//îwåi
	int m_handle;

	//BGM
	int m_bgm;

};

