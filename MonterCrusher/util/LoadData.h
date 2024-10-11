#pragma once
#include <memory>
#include <string>
#include<vector>
class LoadData
{
public:
	LoadData();
	virtual ~LoadData();

	void Init();
	void Updata();
	void Draw();

private:
	void LoadPlayerState();

	void LoadPlayerAnimation();

	void LoadEnemyState();

	void LoadEnemyAnimation();
private:

	std::string m_playerState;
	std::string m_playerAnimation;
	std::string m_enemyState;
	std::string m_enemyAnimation;

	//ƒnƒ“ƒhƒ‹
	int m_enemyStateHandle;

	int m_enemyAnimationHandle;

	int m_playerStateHandle;

	int m_playerAnimationHandle;

};

