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
/// �v���C���[�̃X�e�[�^�X����ǂݍ���
/// </summary>
void LoadData::LoadPlayerState()
{
	std::ifstream file(kPlayerStateFillName);
	std::string line;

	//�t�@�C���̓��͎擾
	//std::getline(�����C�Ãt�@�C���̕ϐ��A���͕�������i�[����ϐ�)
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		m_playerState.clear();
		std::string field;

		//�����񕪊�
		//getline(isstringrream�^�̕ϐ�,����������������i�[����ϐ�,',�ŕ���)
		while (getline(stream, field, ','))
		{
			try
			{
				m_playerState.push_back(std::stof(field));
			}
			catch (const std::invalid_argument& e)
			{
				//�����ȕ�������X�L�b�v
			}
		}

	}

}

void LoadData::LoadPlayerAnimation()
{
	std::ifstream file(kPlayerAnimationFillName);
	std::string line;

	//�t�@�C���̓��͎擾
	//std::getline(�����C�Ãt�@�C���̕ϐ��A���͕�������i�[����ϐ�)
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		m_playerAnimation.clear();
		std::string field;

		//�����񕪊�
		//getline(isstringrream�^�̕ϐ�,����������������i�[����ϐ�,',�ŕ���)
		while (getline(stream, field, ','))
		{
			try
			{
				m_playerAnimation.push_back(std::stof(field));
			}
			catch (const std::invalid_argument& e)
			{
				//�����ȕ�������X�L�b�v
			}
		}

	}
}

void LoadData::LoadEnemyState()
{
	std::ifstream file(kEnemyStateFillName);
	std::string line;

	//�t�@�C���̓��͎擾
	//std::getline(�����C�Ãt�@�C���̕ϐ��A���͕�������i�[����ϐ�)
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		m_enemyState.clear();
		std::string field;

		//�����񕪊�
		//getline(isstringrream�^�̕ϐ�,����������������i�[����ϐ�,',�ŕ���)
		while (getline(stream, field, ','))
		{
			try
			{
				m_enemyState.push_back(std::stof(field));
			}
			catch (const std::invalid_argument& e)
			{
				//�����ȕ�������X�L�b�v
			}
		}

	}
}

void LoadData::LoadEnemyAnimation()
{
	std::ifstream file(kEnemyAnimationFillName);
	std::string line;

	//�t�@�C���̓��͎擾
	//std::getline(�����C�Ãt�@�C���̕ϐ��A���͕�������i�[����ϐ�)
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		m_enemyAnimation.clear();
		std::string field;

		//�����񕪊�
		//getline(isstringrream�^�̕ϐ�,����������������i�[����ϐ�,',�ŕ���)
		while (getline(stream, field, ','))
		{
			try
			{
				m_enemyAnimation.push_back(std::stof(field));
			}
			catch (const std::invalid_argument& e)
			{
				//�����ȕ�������X�L�b�v
			}
		}

	}
}
