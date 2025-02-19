#pragma once
class Stage1;
class StageManager
{
public:
	StageManager();

	virtual ~StageManager();

	void Init();

	void Updata();

	void Draw();

private:
	Stage1* m_pStage1;
private:
	int m_handle;

};

