#include "SceneBase.h"
#include "SceneResult.h"
#include "DxLib.h"
#include "Pad.h"
#include "SceneTitle.h"

SceneResult::SceneResult()
{
}

SceneResult::~SceneResult()
{
	DeleteSoundMem(m_bgm);
}

void SceneResult::Init()
{
	m_bgm = LoadSoundMem("data/BGM/GameOver.mp3");
	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP, true);
}

std::shared_ptr<SceneBase> SceneResult::Update()
{
	// ƒL[“ü—Íæ“¾
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (Pad::IsTrigger(PAD_INPUT_A))
	{
		return std::make_shared<SceneTitle>();
	}
	

	return shared_from_this();
}

void SceneResult::Draw()
{
}

void SceneResult::End()
{
}
