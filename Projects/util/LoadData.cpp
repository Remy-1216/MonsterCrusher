#include "DxLib.h"
#include "LoadData.h"
#include <fstream>
#include <sstream>

namespace
{
	const char* const kPlayerStateFillName = "data/csv/playerstate.csv";

	const char* const kPlayerAnimationFillName = "data/csv/animation.csv";

	const char* const kEnemyStateFillName = "data/csv/enemystate.csv";

	const char* const kEnemyAnimationFillName = "data/csv/animation.csv";
}



LoadData::LoadData()
{
}

LoadData::~LoadData()
{
}

void LoadData::Init()
{
	LoadPlayerState();

	LoadPlayerAnimation();

	LoadEnemyState();

	LoadEnemyAnimation();
}



void LoadData::Updata()
{
	
}

void LoadData::Draw()
{
}

/// <summary>
/// プレイヤーのステータス情報を読み込む
/// </summary>
void LoadData::LoadPlayerState()
{
	std::ifstream file(kPlayerStateFillName);
	std::string line;

	//ファイルの入力取得
	//std::getline(世美呂津ファイルの変数、入力文字列を格納する変数)
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		m_playerState.clear();
		std::string field;

		//文字列分割
		//getline(isstringrream型の変数,分割した文字列を格納する変数,',で分割)
		while (getline(stream, field, ','))
		{
			try
			{
				m_playerState.push_back(std::stof(field));
			}
			catch (const std::invalid_argument& e)
			{
				//無効な文字列をスキップ
			}
		}

	}

}

void LoadData::LoadPlayerAnimation()
{
	std::ifstream file(kPlayerAnimationFillName);
	std::string line;

	//ファイルの入力取得
	//std::getline(世美呂津ファイルの変数、入力文字列を格納する変数)
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		m_playerAnimation.clear();
		std::string field;

		//文字列分割
		//getline(isstringrream型の変数,分割した文字列を格納する変数,',で分割)
		while (getline(stream, field, ','))
		{
			try
			{
				m_playerAnimation.push_back(std::stof(field));
			}
			catch (const std::invalid_argument& e)
			{
				//無効な文字列をスキップ
			}
		}

	}
}

void LoadData::LoadEnemyState()
{
	std::ifstream file(kEnemyStateFillName);
	std::string line;

	//ファイルの入力取得
	//std::getline(世美呂津ファイルの変数、入力文字列を格納する変数)
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		m_enemyState.clear();
		std::string field;

		//文字列分割
		//getline(isstringrream型の変数,分割した文字列を格納する変数,',で分割)
		while (getline(stream, field, ','))
		{
			try
			{
				m_enemyState.push_back(std::stof(field));
			}
			catch (const std::invalid_argument& e)
			{
				//無効な文字列をスキップ
			}
		}

	}
}

void LoadData::LoadEnemyAnimation()
{
	std::ifstream file(kEnemyAnimationFillName);
	std::string line;

	//ファイルの入力取得
	//std::getline(世美呂津ファイルの変数、入力文字列を格納する変数)
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		m_enemyAnimation.clear();
		std::string field;

		//文字列分割
		//getline(isstringrream型の変数,分割した文字列を格納する変数,',で分割)
		while (getline(stream, field, ','))
		{
			try
			{
				m_enemyAnimation.push_back(std::stof(field));
			}
			catch (const std::invalid_argument& e)
			{
				//無効な文字列をスキップ
			}
		}

	}
}
