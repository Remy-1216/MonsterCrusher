#include "DxLib.h"
#include "StageBase.h"

namespace
{
	//�E����������
	constexpr float kRotateH = 90.0f;

	//���W�A���ɕύX���邽�߂̌v�Z�ɕK�v�Ȑ���
	constexpr float kDegrees = 180.0f;
}

StageBase::StageBase(int ModelHandle)
{
	m_handle = ModelHandle;
}

StageBase::~StageBase()
{
	MV1DeleteModel(m_handle);
}

void StageBase::Init()
{
	//���f������]�����āA�E����������
	MV1SetRotationXYZ(m_handle, VGet(0.0f, kRotateH * DX_PI_F / kDegrees, 0.0f));
}

void StageBase::Draw()
{
	//�|�W�V�����̐ݒ�
	MV1SetPosition(m_handle, m_pos);

	// �R�c���f���̕`��
	MV1DrawModel(m_handle);
}
