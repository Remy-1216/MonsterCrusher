#pragma once
#include <memory>

class SceneBase;
class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();

	void Init();
	void Update();
	void Draw();
	void End();
private:
	//	SceneBase* m_pScene;
	std::shared_ptr<SceneBase>m_pScene;
};

