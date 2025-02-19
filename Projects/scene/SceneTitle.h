#pragma once
#include "DxLib.h"
#include "SceneBase.h"


class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

private:

	//�A�j���[�V�����֘A
	void Animation();

private:
	//�n���h��
	int m_handle;

	//�n�`�̃n���h��
	int m_beeHandle;

	//�X���C���̃n���h��
	int m_slimeHandle;

	//�A�^�b�`�����A�j���[�V�����̍Đ����Ԃ�ۑ�����
	int m_totalTime;

	//�A�^�b�`�����n�`�̃A�j���[�V�����̍Đ����Ԃ�ێ�����
	int m_beeTotalTime;

	//�A�^�b�`�����X���C���̃A�j���[�V�����̍Đ����Ԃ�ێ�����
	int m_slimeTotalTime;

	//�i�C�g�̃A�j���[�V�����i���o�[
	int m_attachIndex;

	//�n�`�̃A�j���[�V�����i���o�[
	int m_beeAttachIndex;

	//�X���C���A�j���[�V�����i���o�[
	int m_slimeAttachIndex;

	//�i�C�g�̍Đ�����
	float m_playTime;

	//�n�`�̍Đ�����
	float m_beePlayTime;

	//�X���C���̍Đ�����
	float m_slimePlayTime;

	//�t�F�[�h�C���A�A�E�g
	int m_fadeAlpha;

	//���艹
	int m_decisionSE;

	//���f��
	int m_modelHandle;

	//BGM
	int m_bgm;

	//�V�[���I���t���O
	bool m_isSceneEnd;	

	//���f���̍��W
	VECTOR m_pos;
	
	//�n�`�̍��W
	VECTOR m_beePos;

	//�X���C���̍��W
	VECTOR m_slimePos;

	//�J�����̍��W
	VECTOR m_cameraPos;
	VECTOR m_cameraTarget;
};

