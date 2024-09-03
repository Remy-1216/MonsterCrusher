#include "DxLib.h"
#include "StageManager.h"
#include "Stage1.h"

StageManager::StageManager()
{
}

StageManager::~StageManager()
{
	MV1DeleteModel(m_handle);
}

void StageManager::Init()
{
	m_handle = MV1LoadModel("data/model/stage/kari.mv1");
	m_pStage1 = new Stage1(m_handle);
	m_pStage1->Init();
}

void StageManager::Updata()
{
	m_pStage1->Update();
}

void StageManager::Draw()
{
	m_pStage1->Draw();
}
